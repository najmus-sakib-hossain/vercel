use crossterm::{
    event::{self, DisableMouseCapture, EnableMouseCapture, Event, KeyCode},
    execute,
    terminal::{disable_raw_mode, enable_raw_mode, EnterAlternateScreen, LeaveAlternateScreen},
};
use ratatui::{
    backend::{Backend, CrosstermBackend},
    layout::{Constraint, Direction, Layout, Rect},
    style::{Color, Modifier, Style},
    text::{Line, Span, Text},
    widgets::{
        block::Title, Block, Borders, List, ListItem, ListState, Paragraph, Wrap,
    },
    Frame, Terminal,
};
use similar::{ChangeTag, TextDiff};
use std::{error::Error, io};
use syntect::{
    easy::HighlightLines,
    highlighting::{ThemeSet},
    parsing::SyntaxSet,
    util::LinesWithEndings,
};

const OLD_CODE: &str = r#"
import React from "react";

function Greeting({ name }) {
  return <h1>Hello, {name}!</h1>;
}

export default Greeting;
"#;

const NEW_CODE: &str = r#"
import React, { useState } from "react";

function Greeting({ name }) {
  const [greeting, setGreeting] = useState(`Hello, ${name}!`);

  return (
    <div>
      <h1>{greeting}</h1>
      <button onClick={() => setGreeting("Hi there!")}>Change Greeting</button>
    </div>
  );
}

export default Greeting;
"#;

#[derive(Clone)]
struct Theme {
    name: String,
    background: Color,
    foreground: Color,
    primary: Color,
    primary_foreground: Color,
    destructive: Color,
    border: Color,
    diff_add_bg: Color,
    diff_delete_bg: Color,
}

impl Theme {
    fn default_themes() -> Vec<Theme> {
        vec![
            Theme {
                name: "Dark".to_string(),
                background: Color::Rgb(0, 0, 0),
                foreground: Color::Rgb(252, 252, 252),
                primary: Color::Rgb(252, 252, 252),
                primary_foreground: Color::Rgb(12, 12, 13),
                destructive: Color::Rgb(228, 51, 62),
                border: Color::Rgb(41, 41, 45),
                diff_add_bg: Color::Rgb(10, 40, 10),
                diff_delete_bg: Color::Rgb(50, 20, 20),
            },
            Theme {
                name: "Light".to_string(),
                background: Color::Rgb(255, 255, 255),
                foreground: Color::Rgb(12, 12, 13),
                primary: Color::Rgb(12, 12, 13),
                primary_foreground: Color::Rgb(252, 252, 252),
                destructive: Color::Rgb(239, 68, 68),
                border: Color::Rgb(234, 234, 235),
                diff_add_bg: Color::Rgb(220, 255, 220),
                diff_delete_bg: Color::Rgb(255, 220, 220),
            },
            Theme {
                name: "Rose".to_string(),
                background: Color::Rgb(68, 20, 26),
                foreground: Color::Rgb(255, 228, 230),
                primary: Color::Rgb(253, 164, 175),
                primary_foreground: Color::Rgb(68, 20, 26),
                destructive: Color::Rgb(244, 63, 94),
                border: Color::Rgb(159, 18, 57),
                diff_add_bg: Color::Rgb(20, 40, 20),
                diff_delete_bg: Color::Rgb(60, 30, 30),
            },
        ]
    }
}

struct App {
    themes: Vec<Theme>,
    theme_list_state: ListState,
}

impl App {
    fn new() -> App {
        let themes = Theme::default_themes();
        let mut theme_list_state = ListState::default();
        theme_list_state.select(Some(0));

        App {
            themes,
            theme_list_state,
        }
    }

    pub fn current_theme(&self) -> &Theme {
        self.themes
            .get(self.theme_list_state.selected().unwrap_or(0))
            .unwrap()
    }

    pub fn next_theme(&mut self) {
        let i = match self.theme_list_state.selected() {
            Some(i) => {
                if i >= self.themes.len() - 1 {
                    0
                } else {
                    i + 1
                }
            }
            None => 0,
        };
        self.theme_list_state.select(Some(i));
    }

    pub fn previous_theme(&mut self) {
        let i = match self.theme_list_state.selected() {
            Some(i) => {
                if i == 0 {
                    self.themes.len() - 1
                } else {
                    i - 1
                }
            }
            None => 0,
        };
        self.theme_list_state.select(Some(i));
    }
}

fn main() -> Result<(), Box<dyn Error>> {
    enable_raw_mode()?;
    let mut stdout = io::stdout();
    execute!(stdout, EnterAlternateScreen, EnableMouseCapture)?;
    let backend = CrosstermBackend::new(stdout);
    let mut terminal = Terminal::new(backend)?;

    let app = App::new();
    let res = run_app(&mut terminal, app);

    disable_raw_mode()?;
    execute!(
        terminal.backend_mut(),
        LeaveAlternateScreen,
        DisableMouseCapture
    )?;
    terminal.show_cursor()?;

    if let Err(err) = res {
        println!("{err:?}");
    }

    Ok(())
}

fn run_app<B: Backend>(terminal: &mut Terminal<B>, mut app: App) -> io::Result<()> {
    loop {
        terminal.draw(|f| ui(f, &mut app))?;

        if let Event::Key(key) = event::read()? {
            match key.code {
                KeyCode::Char('q') => return Ok(()),
                KeyCode::Down => app.next_theme(),
                KeyCode::Up => app.previous_theme(),
                _ => {}
            }
        }
    }
}

fn ui<B: Backend>(f: &mut Frame<B>, app: &mut App) {
    let current_theme = app.current_theme().clone();

    f.render_widget(
        Block::default().style(Style::default().bg(current_theme.background)),
        f.size(),
    );

    let chunks = Layout::default()
        .direction(Direction::Horizontal)
        .constraints([Constraint::Percentage(30), Constraint::Percentage(70)].as_ref())
        .split(f.size());

    render_theme_list(f, app, chunks[0], &current_theme);
    render_diff_view(f, chunks[1], &current_theme);
}

fn render_theme_list<B: Backend>(f: &mut Frame<B>, app: &mut App, area: Rect, theme: &Theme) {
    let items: Vec<ListItem> = app
        .themes
        .iter()
        .map(|t| ListItem::new(t.name.as_str()).style(Style::default().fg(theme.foreground)))
        .collect();

    let list = List::new(items)
        .block(
            Block::default()
                .borders(Borders::ALL)
                .border_style(Style::default().fg(theme.border))
                .title(Title::from(Span::styled(
                    "Themes",
                    Style::default().fg(theme.foreground),
                ))),
        )
        .highlight_style(
            Style::default()
                .bg(theme.primary)
                .fg(theme.primary_foreground)
                .add_modifier(Modifier::BOLD),
        )
        .highlight_symbol(">> ");

    f.render_stateful_widget(list, area, &mut app.theme_list_state);
}

fn render_diff_view<B: Backend>(f: &mut Frame<B>, area: Rect, theme: &Theme) {
    let diff_text = create_diff_text(OLD_CODE, NEW_CODE, theme);

    let paragraph = Paragraph::new(diff_text)
        .wrap(Wrap { trim: false })
        .block(
            Block::default()
                .borders(Borders::ALL)
                .border_style(Style::default().fg(theme.border))
                .title(Title::from(Span::styled(
                    "Diff Preview",
                    Style::default().fg(theme.foreground),
                ))),
        );

    f.render_widget(paragraph, area);
}

fn create_diff_text<'a>(old: &'a str, new: &'a str, theme: &Theme) -> Text<'a> {
    let ps = SyntaxSet::load_defaults_newlines();
    let ts = ThemeSet::load_defaults();

    let syntax = ps
        .find_syntax_by_extension("tsx")
        .unwrap_or_else(|| ps.find_syntax_plain_text());

    let syntect_theme = &ts.themes["base16-ocean.dark"];
    let mut h = HighlightLines::new(syntax, syntect_theme);

    let diff = TextDiff::from_lines(old, new);
    let mut lines = Vec::new();

    for change in diff.iter_all_changes() {
        let (marker, marker_style, bg_color) = match change.tag() {
            ChangeTag::Delete => (
                "-",
                Style::default().fg(theme.destructive),
                Some(theme.diff_delete_bg),
            ),
            ChangeTag::Insert => (
                "+",
                Style::default().fg(Color::Green),
                Some(theme.diff_add_bg),
            ),
            ChangeTag::Equal => (" ", Style::default().fg(Color::DarkGray), None),
        };

        if let Some(value) = change.as_str() {
            for line in LinesWithEndings::from(value) {
                let spans: Vec<Span> = match h.highlight_line(line, &ps) {
                    Ok(ranges) => ranges
                        .into_iter()
                        .map(|(style, content)| {
                            let color = style.foreground;
                            Span::styled(
                                content.to_string(),
                                Style::default().fg(Color::Rgb(color.r, color.g, color.b)),
                            )
                        })
                        .collect(),
                    Err(_) => vec![Span::raw(line)],
                };

                let mut line_spans = vec![Span::styled(marker.to_string() + " ", marker_style)];
                line_spans.extend(spans);

                let mut styled_line = Line::from(line_spans);

                if let Some(bg) = bg_color {
                    styled_line.patch_style(Style::default().bg(bg));
                } else {
                    styled_line.patch_style(Style::default().fg(theme.foreground));
                }
                lines.push(styled_line);
            }
        }
    }

    Text::from(lines)
}