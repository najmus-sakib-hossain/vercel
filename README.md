# dx
https://github.com/manfromexistence/dx-forge.git

Good, combine Asynchronous I/O (Advanced) with Combine Operations into a Single Pass to make the rust fastest i/o possible!!!

# Set up tmpfs
sudo mkdir /mnt/tmpfs
sudo mount -t tmpfs -o size=1G tmpfs /mnt/tmpfs
cd /mnt/tmpfs


cd /dev/shm || cd /tmp
gcc -O3 -pthread fastest_io.c -o fastest_io
./fastest_io

gcc -O3 -pthread fast_io.c -o fast_io
./fast_io

git init && git add . && git commit -m "feat: dx" && git branch -M main && git remote add origin https://github.com/najmus-sakib-hossain/vercel.git && git push -u origin main
find . -maxdepth 1 -mindepth 1 -type d -exec du -sh {} + | sed 's/K/KB/; s/M/MB/; s|\./||'
find . -type d -name "tests" -exec rm -r {} +
find . -maxdepth 1 -mindepth 1 -exec du -sh {} + | sed 's/K/KB/; s/M/MB/; s|\./||'
find . -maxdepth 1 -mindepth 1 ! -name "cli" ! -name "src" ! -name "creates" ! -name "packages" -exec rm -rf {} +

# Inspirations
```
git clone https://github.com/biomejs/biomet && cd biome && rm -rf .git && cd ..
git clone https://github.com/cross-rs/cross && cd cross && rm -rf .git && cd ..
git clone https://github.com/casey/just && cd just && rm -rf .git && cd ..
git clone https://github.com/console-rs/indicatif && cd indicatif && rm -rf .git && cd ..
git clone https://github.com/LinusU/rust-log-update && cd rust-log-update && rm -rf .git && cd ..
git clone https://github.com/VincentFoulon80/console_engine && cd console_engine && rm -rf .git && cd ..
git clone https://github.com/latipun7/charasay && cd charasay && rm -rf .git && cd ..
git clone https://github.com/nukesor/comfy-table && cd comfy-table && rm -rf .git && cd ..
git clone https://github.com/manfromexistence/ui && cd ui && rm -rf .git && cd ..
git clone https://github.com/neovim/neovim && cd neovim && rm -rf .git && cd ..
git clone https://github.com/ghostty-org/ghostty && cd ghostty && rm -rf .git && cd ..
git clone https://github.com/redox-os/ion.git && cd ion && rm -rf .git && cd ..
git clone https://github.com/ohmyzsh/ohmyzsh && cd ohmyzsh && rm -rf .git && cd ..
git clone https://github.com/shadcn-ui/ui && cd claude-code && rm -rf .git && cd ..
git clone https://github.com/anthropics/claude-code && cd claude-code && rm -rf .git && cd ..
git clone https://github.com/ratatui/ratatui && cd ratatui && rm -rf .git && cd ..
git clone https://github.com/google-gemini/gemini-cli && cd gemini-cli && rm -rf .git && cd ..
git clone https://github.com/mikaelmello/inquire && cd inquire && rm -rf .git && cd ..
git clone https://github.com/bombshell-dev/clack && cd clack && rm -rf .git && cd ..
git clone https://github.com/oven-sh/bun && cd bun && rm -rf .git && cd ..
git clone https://github.com/haydenbleasel/ultracite.git && cd ultracite && rm -rf .git && cd ..
git clone https://github.com/tailwindlabs/tailwindcss && cd tailwindcss && rm -rf .git && cd ..
```

<!-- use syntect::easy::HighlightLines;
use syntect::parsing::SyntaxSet;
use syntect::highlighting::{ThemeSet, Style};
use syntect::util::{as_24_bit_terminal_escaped, LinesWithEndings};

fn main() {
    let ps = SyntaxSet::load_defaults_nonewlines();
    let ts = ThemeSet::load_defaults();

    let syntax = ps.find_syntax_by_extension("rs")
        .expect("Could not find Rust syntax. Check syntect features in Cargo.toml.");

    // Safely get the theme, or use a reliable fallback if it's not found.
    let theme = ts.themes.get("Monokai (Dark)")
        .unwrap_or_else(|| &ts.themes["base16-ocean.dark"]);

    let code = r#"
fn main() {
    println!("Hello, from syntect!");
}
"#;

    println!("--- Start of Highlighted Code ---");
    let mut h = HighlightLines::new(syntax, theme);
    for line in LinesWithEndings::from(code) {
        let ranges: Vec<(Style, &str)> = h.highlight_line(line, &ps).unwrap();
        let escaped = as_24_bit_terminal_escaped(&ranges[..], true);
        print!("{}", escaped);
    }
    println!("--- End of Highlighted Code ---");
} -->

<!-- 
use lolcrab::Lolcrab;
use std::io;

const TEXT: &str = "\
•••••••••••••••••••••••••••••••••••••••••••
••442463299144744830108724702438783348716••
••665891426009540978622724448305819269356••
••078289454141226451790882961903610719673••
••56505384476•••••••••••••••••39761609699••
••47928752907•• { lolcrab } ••33810561851••
••51609982385•••••••••••••••••43459368213••
••980457234663167653959566555465520046709••
••677103598707232478714861999441705454744••
••012721882924436718718457599087686681354••
•••••••••••••••••••••••••••••••••••••••••••
";

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let stdout = io::stdout();
    let mut stdout = stdout.lock();

    // Initialize Lolcrab using default gradient and default noise
    let mut lol = Lolcrab::new(None, None);

    lol.colorize_str(TEXT, &mut stdout)?;

    lol.set_invert(true);
    lol.randomize_position();
    lol.colorize_str(TEXT, &mut stdout)?;

    lol.set_invert(false);
    lol.reset_position();
    lol.colorize_str(TEXT, &mut stdout)?;

    Ok(())
} -->

<!-- 
use figlet_rs::FIGfont;
use lolcrab::Lolcrab;
use std::io::{self, Write};

fn main() {
    // 1. Create the big text with figlet
    let font = FIGfont::standard().unwrap();
    let figlet_text = font.convert("dx").unwrap();
    let figlet_string = figlet_text.to_string();

    // 2. Initialize Lolcrab
    let mut lol = Lolcrab::new(None, None);

    // 3. Get a handle to the terminal output
    let stdout = io::stdout();
    let mut handle = stdout.lock();

    // 4. Clear the screen to ensure a clean display
    write!(handle, "\x1B[2J\x1B[1;1H").unwrap();

    // 5. Colorize the text and print it to the terminal just once
    lol.colorize_str(&figlet_string, &mut handle).unwrap();
} 
-->


<!-- 
use dx::Text;

fn main() {
    let name = Text::new("What command you want to run?").prompt();

    match name {
        Ok(name) => println!("Command [{name}] is still in developement - it is coming soon..."),
        Err(_) => println!("An error happened when running this command, try again later."),
    }
}

mod chronicle;
mod generator;
mod observer;

#[tokio::main]
async fn main() -> anyhow::Result<()> {
    println!("DX: Initializing...");

    let chronicle_repo = match chronicle::initialize() {
        Ok(repo) => repo,
        Err(e) => {
            eprintln!("DX Error: Failed to initialize the Chronicle: {}", e);
            return Err(e);
        }
    };

    if let Err(e) = observer::start(chronicle_repo.clone()).await {
        eprintln!("DX Error: The observer failed with an error: {}", e);
    }

    println!("DX: Shutting down.");
    Ok(())
} 
-->
