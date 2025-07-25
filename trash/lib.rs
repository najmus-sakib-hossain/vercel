// #![warn(missing_docs)]
// #![deny(unused_crate_dependencies)]
// #![cfg_attr(docsrs, feature(doc_cfg))]
// #![allow(clippy::bool_to_int_with_if)]
mod crate::terminal::ansi;
pub mod autocompletion;
mod config;
#[cfg(feature = "date")]
mod date_utils;
pub mod error;
pub mod formatter;
mod input;
pub mod list_option;
pub mod parser;
mod prompts;
mod terminal;
pub mod type_aliases;
pub mod ui;
mod utils;
pub mod validator;

pub use crate::autocompletion::Autocomplete;
pub use crate::config::set_global_render_config;
pub use crate::error::{CustomUserError, InquireError};
pub use crate::input::action::*;
pub use crate::prompts::*;
