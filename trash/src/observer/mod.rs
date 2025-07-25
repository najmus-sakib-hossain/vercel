use crate::{chronicle, generator};
use anyhow::Result;
use notify::{RecursiveMode, Watcher};
use std::path::Path;
use tokio::sync::mpsc;

pub async fn start(mut chronicle_repo: gix::Repository) -> Result<()> {
    println!("DX Observer: Initializing...");

    let path_to_watch = ".";
    println!(
        "DX Observer: Watching directory -> {}",
        Path::new(path_to_watch).canonicalize()?.display()
    );

    let (tx, mut rx) = mpsc::unbounded_channel();

    let mut watcher = notify::recommended_watcher(move |res: notify::Result<notify::Event>| {
        if let Ok(event) = res {
            if tx.send(event).is_err() {
                eprintln!("Warning: Channel receiver has been dropped, cannot send event.");
            }
        }
    })?;

    watcher.watch(Path::new(path_to_watch), RecursiveMode::Recursive)?;

    println!("DX Observer: Watcher is active. Waiting for file changes...");
    println!("(Try creating/modifying a .tsx file with a <Dx.IconName /> tag)");

    while let Some(event) = rx.recv().await {
        if event.kind.is_modify() || event.kind.is_create() {
            let tsx_paths: Vec<_> = event
                .paths
                .into_iter()
                .filter(|path| path.extension().map_or(false, |ext| ext == "tsx"))
                .collect();

            if !tsx_paths.is_empty() {
                println!("\n[TSX EVENT DETECTED] >> {:?}", event.kind);
                for path in tsx_paths {
                    println!("  -> Path: {}", path.display());

                    if let Some(path_str) = path.to_str() {
                        let generated = generator::process_file(path_str);

                        if generated {
                            println!("  -> ACTION: Zig generator successfully created a new component.");
                            if let Err(e) =
                                chronicle::record_change(&mut chronicle_repo, &path, "green")
                            {
                                eprintln!("  -> CHRONICLE ERROR: Failed to commit change: {}", e);
                            }
                        } else {
                            println!(
                                "  -> INFO: No <Dx.*> component found to generate in this change."
                            );
                        }
                    }
                }
            }
        }
    }
    Ok(())
}
