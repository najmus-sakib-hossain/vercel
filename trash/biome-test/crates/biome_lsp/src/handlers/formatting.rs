use crate::diagnostics::LspError;
use crate::session::Session;
use crate::utils::text_edit;
use anyhow::Context;
use biome_fs::BiomePath;
use biome_lsp_converters::from_proto;
use biome_rowan::{TextLen, TextRange, TextSize};
use biome_service::file_handlers::{AstroFileHandler, SvelteFileHandler, VueFileHandler};
use biome_service::workspace::{
    CheckFileSizeParams, FeaturesBuilder, FeaturesSupported, FileFeaturesResult, FormatFileParams,
    FormatOnTypeParams, FormatRangeParams, GetFileContentParams, IgnoreKind, IsPathIgnoredParams,
    SupportsFeatureParams,
};
use biome_service::{WorkspaceError, extension_error};
use std::ops::Sub;
use tower_lsp_server::lsp_types::*;

#[tracing::instrument(level = "debug", skip(session), err)]
pub(crate) fn format(
    session: &Session,
    params: DocumentFormattingParams,
) -> Result<Option<Vec<TextEdit>>, LspError> {
    let url = params.text_document.uri;
    let path = session.file_path(&url)?;
    let Some(doc) = session.document(&url) else {
        return Ok(None);
    };
    if !session.workspace.file_exists(path.clone().into())? {
        return Ok(None);
    }
    let features = FeaturesBuilder::new().with_formatter().build();

    if session.workspace.is_path_ignored(IsPathIgnoredParams {
        path: path.clone(),
        project_key: doc.project_key,
        features,
        ignore_kind: IgnoreKind::Ancestors,
    })? {
        return Ok(None);
    }
    let features = FeaturesBuilder::new().with_formatter().build();
    let FileFeaturesResult {
        features_supported: file_features,
    } = session.workspace.file_features(SupportsFeatureParams {
        project_key: doc.project_key,
        path: path.clone(),
        features,
    })?;

    if file_features.supports_format()
        && !session.workspace.is_path_ignored(IsPathIgnoredParams {
            path: path.clone(),
            project_key: doc.project_key,
            features,
            ignore_kind: IgnoreKind::Ancestors,
        })?
    {
        let size_limit_result = session.workspace.check_file_size(CheckFileSizeParams {
            project_key: doc.project_key,
            path: path.clone(),
        })?;
        if size_limit_result.is_too_large() {
            return Ok(None);
        }

        let printed = session.workspace.format_file(FormatFileParams {
            project_key: doc.project_key,
            path: path.clone(),
        })?;

        let mut output = printed.into_code();
        let input = session.workspace.get_file_content(GetFileContentParams {
            project_key: doc.project_key,
            path: path.clone(),
        })?;
        if output.is_empty() {
            return Ok(None);
        }
        match path.extension() {
            Some("astro") => {
                output = AstroFileHandler::output(input.as_str(), output.as_str());
            }
            Some("vue") => {
                output = VueFileHandler::output(input.as_str(), output.as_str());
            }
            Some("svelte") => {
                output = SvelteFileHandler::output(input.as_str(), output.as_str());
            }
            _ => {}
        }

        let indels = biome_text_edit::TextEdit::from_unicode_words(input.as_str(), output.as_str());
        let position_encoding = session.position_encoding();
        let edits = text_edit(&doc.line_index, indels, position_encoding, None)?;

        Ok(Some(edits))
    } else {
        notify_user(file_features, path)
    }
}

#[tracing::instrument(level = "debug", skip(session), err)]
pub(crate) fn format_range(
    session: &Session,
    params: DocumentRangeFormattingParams,
) -> Result<Option<Vec<TextEdit>>, LspError> {
    let url = params.text_document.uri;
    let path = session.file_path(&url)?;
    let Some(doc) = session.document(&url) else {
        return Err(extension_error(&path).into());
    };
    if !session.workspace.file_exists(path.clone().into())? {
        return Ok(None);
    }
    let features = FeaturesBuilder::new().with_formatter().build();

    if session.workspace.is_path_ignored(IsPathIgnoredParams {
        path: path.clone(),
        project_key: doc.project_key,
        features,
        ignore_kind: IgnoreKind::Ancestors,
    })? {
        return Ok(None);
    }

    let features = FeaturesBuilder::new().with_formatter().build();
    let FileFeaturesResult {
        features_supported: file_features,
    } = session.workspace.file_features(SupportsFeatureParams {
        project_key: doc.project_key,
        path: path.clone(),
        features,
    })?;

    if file_features.supports_format()
        && !session.workspace.is_path_ignored(IsPathIgnoredParams {
            path: path.clone(),
            project_key: doc.project_key,
            features,
            ignore_kind: IgnoreKind::Ancestors,
        })?
    {
        let size_limit_result = session.workspace.check_file_size(CheckFileSizeParams {
            project_key: doc.project_key,
            path: path.clone(),
        })?;
        if size_limit_result.is_too_large() {
            return Ok(None);
        }

        let position_encoding = session.position_encoding();
        let format_range = from_proto::text_range(&doc.line_index, params.range, position_encoding)
            .with_context(|| {
                format!(
                    "failed to convert range {:?} in document {}",
                    params.range.end,
                    url.as_str()
                )
            })?;
        let content = session.workspace.get_file_content(GetFileContentParams {
            project_key: doc.project_key,
            path: path.clone(),
        })?;
        let offset = match path.extension() {
            Some("vue") => VueFileHandler::start(content.as_str()),
            Some("astro") => AstroFileHandler::start(content.as_str()),
            Some("svelte") => SvelteFileHandler::start(content.as_str()),
            _ => None,
        };
        let format_range = if let Some(offset) = offset {
            if format_range.start() - TextSize::from(offset) >= TextSize::from(0) {
                TextRange::new(
                    format_range.start().sub(TextSize::from(offset)),
                    format_range.end().sub(TextSize::from(offset)),
                )
            } else {
                format_range
            }
        } else {
            format_range
        };

        let formatted = session.workspace.format_range(FormatRangeParams {
            project_key: doc.project_key,
            path: path.clone(),
            range: format_range,
        })?;

        let formatted_range = formatted
            .range()
            .unwrap_or_else(|| TextRange::up_to(content.text_len()));
        let indels = biome_text_edit::TextEdit::from_unicode_words(
            &content.as_str()[formatted_range],
            formatted.as_code(),
        );
        let position_encoding = session.position_encoding();
        let edits = text_edit(
            &doc.line_index,
            indels,
            position_encoding,
            Some(formatted_range.start().into()),
        )?;

        Ok(Some(edits))
    } else {
        notify_user(file_features, path)
    }
}

#[tracing::instrument(level = "debug", skip(session), err)]
pub(crate) fn format_on_type(
    session: &Session,
    params: DocumentOnTypeFormattingParams,
) -> Result<Option<Vec<TextEdit>>, LspError> {
    let url = params.text_document_position.text_document.uri;
    let position = params.text_document_position.position;
    let path = session.file_path(&url)?;
    let Some(doc) = session.document(&url) else {
        return Err(extension_error(&path).into());
    };
    if !session.workspace.file_exists(path.clone().into())? {
        return Ok(None);
    }
    let features = FeaturesBuilder::new().with_formatter().build();
    let FileFeaturesResult {
        features_supported: file_features,
    } = session.workspace.file_features(SupportsFeatureParams {
        project_key: doc.project_key,
        path: path.clone(),
        features,
    })?;

    if session.workspace.is_path_ignored(IsPathIgnoredParams {
        path: path.clone(),
        project_key: doc.project_key,
        features,
        ignore_kind: IgnoreKind::Ancestors,
    })? {
        return notify_user(file_features, path);
    }

    if file_features.supports_format()
        && !session.workspace.is_path_ignored(IsPathIgnoredParams {
            path: path.clone(),
            project_key: doc.project_key,
            features,
            ignore_kind: IgnoreKind::Ancestors,
        })?
    {
        let size_limit_result = session.workspace.check_file_size(CheckFileSizeParams {
            project_key: doc.project_key,
            path: path.clone(),
        })?;
        if size_limit_result.is_too_large() {
            return Ok(None);
        }

        let position_encoding = session.position_encoding();
        let offset = from_proto::offset(&doc.line_index, position, position_encoding)
            .with_context(|| {
                format!(
                    "failed to access position {position:?} in document {}",
                    url.as_str()
                )
            })?;

        let formatted = session.workspace.format_on_type(FormatOnTypeParams {
            project_key: doc.project_key,
            path: path.clone(),
            offset,
        })?;

        let content = session.workspace.get_file_content(GetFileContentParams {
            project_key: doc.project_key,
            path: path.clone(),
        })?;

        let formatted_range = formatted
            .range()
            .unwrap_or_else(|| TextRange::up_to(content.text_len()));
        let indels = biome_text_edit::TextEdit::from_unicode_words(
            &content.as_str()[formatted_range],
            formatted.as_code(),
        );
        let edits = text_edit(
            &doc.line_index,
            indels,
            position_encoding,
            Some(formatted_range.start().into()),
        )?;
        Ok(Some(edits))
    } else {
        notify_user(file_features, path)
    }
}

fn notify_user<T>(file_features: FeaturesSupported, biome_path: BiomePath) -> Result<T, LspError> {
    let error = if file_features.is_ignored() {
        WorkspaceError::file_ignored(biome_path.to_string())
    } else if file_features.is_protected() {
        WorkspaceError::protected_file(biome_path.to_string())
    } else {
        extension_error(&biome_path)
    };

    Err(error.into())
}
