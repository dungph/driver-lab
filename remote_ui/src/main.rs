mod qt_obj;

use cxx_qt_lib::{QGuiApplication, QQmlApplicationEngine, QUrl};

pub fn run_command(hostname: &str, command: &str) -> std::io::Result<String> {
    let cmd = format!("ssh root@{hostname} '{command}'");
    Ok(String::from_utf8_lossy(
        &std::process::Command::new("sh")
            .arg("-c")
            .arg(cmd)
            .output()?
            .stdout,
    )
    .to_string())
}
pub fn get_attr(hostname: &str, attribute: &str) -> std::io::Result<String> {
    let cmd = format!("cat /sys/kernel/i2c1602/{attribute}");
    run_command(hostname, &cmd)
}
pub fn set_attr(hostname: &str, attribute: &str, value: &str) -> std::io::Result<String> {
    let cmd = format!("echo \"{value}\" > /sys/kernel/i2c1602/{attribute}");
    run_command(hostname, &cmd)
}

fn main() {
    let mut app = QGuiApplication::new();
    let mut engine = QQmlApplicationEngine::new();

    if let Some(engine) = engine.as_mut() {
        engine.load(&QUrl::from("qrc:/main.qml"));
    }
    if let Some(app) = app.as_mut() {
        app.exec();
    }
}
