#![warn(missing_docs)]
#![deny(unused_crate_dependencies)]
#![cfg_attr(docsrs, feature(doc_cfg))]
#![allow(clippy::bool_to_int_with_if)]
mod ansi;
mod config;
mod date_utils;
mod input;
pub mod autocompletion;
mod prompts;
mod terminal;
mod utils;
#[cfg(feature = "date")]
pub mod error;
pub mod formatter;
pub mod list_option;
pub mod parser;
pub mod type_aliases;
pub mod ui;
pub mod validator;

pub use crate::autocompletion::Autocomplete;
pub use crate::config::set_global_render_config;
pub use crate::error::{CustomUserError, InquireError};
pub use crate::input::action::*;
pub use crate::prompts::*;
