use figlet_rs::FIGfont;
use lolcrab::Lolcrab;
use std::io::{self, Write};

fn main() {
    let font = FIGfont::standard().unwrap();
    let figlet_text = font.convert("dx").unwrap();
    let figlet_string = figlet_text.to_string();

    let mut lol = Lolcrab::new(None, None);

    let stdout = io::stdout();
    let mut handle = stdout.lock();

    write!(handle, "\x1B[2J\x1B[1;1H").unwrap();

    lol.colorize_str(&figlet_string, &mut handle).unwrap();
}