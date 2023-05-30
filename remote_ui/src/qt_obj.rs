const DRIVER_ATTRIBUTES: &[&str] = &[
    "auto_newline",
    "backlight",
    "blink",
    "content",
    "cursor",
    "display",
    "line1",
    "line2",
    "scroll",
];

#[cxx_qt::bridge]
mod qt_obj {
    // ANCHOR: book_qstring_import
    unsafe extern "C++" {
        include!("cxx-qt-lib/qstring.h");
        /// An alias to the QString type
        type QString = cxx_qt_lib::QString;
    }

    #[cxx_qt::qobject(qml_uri = "io.demo", qml_version = "1.0")]
    #[derive(Default)]
    pub struct Demo {
        #[qproperty]
        blink: bool,

        #[qproperty]
        cursor: bool,

        #[qproperty]
        display: bool,

        #[qproperty]
        backlight: bool,

        #[qproperty]
        auto_newline: bool,

        #[qproperty]
        scroll: bool,

        #[qproperty]
        logged_in: bool,

        #[qproperty]
        status_label: QString,

        #[qproperty]
        content: QString,

        hostname: String,
    }

    impl qobject::Demo {
        #[qinvokable]
        pub fn try_hostname(mut self: Pin<&mut Self>, hostname: &QString) {
            let success = crate::run_command(&hostname.to_string(), "ls /sys/kernel/i2c1602")
                .map(|s| {
                    s.lines()
                        .all(|s| super::DRIVER_ATTRIBUTES.iter().any(|t| *t == s))
                        && s.lines().count() >= super::DRIVER_ATTRIBUTES.len()
                })
                .unwrap_or(false);
            if success {
                self.as_mut().set_hostname(hostname.to_string());
                self.as_mut().set_logged_in(true);
                self.as_mut().set_blink(
                    crate::get_attr(&hostname.to_string(), "blink")
                        .map(|s| s == "1")
                        .unwrap_or(false),
                );
                self.as_mut().set_cursor(
                    crate::get_attr(&hostname.to_string(), "cursor")
                        .map(|s| s == "1")
                        .unwrap_or(false),
                );
                self.as_mut().set_display(
                    crate::get_attr(&hostname.to_string(), "display")
                        .map(|s| s == "1")
                        .unwrap_or(false),
                );
                self.as_mut().set_backlight(
                    crate::get_attr(&hostname.to_string(), "backlight")
                        .map(|s| s == "1")
                        .unwrap_or(false),
                );
                self.as_mut().set_scroll(
                    crate::get_attr(&hostname.to_string(), "scroll")
                        .map(|s| s == "1")
                        .unwrap_or(false),
                );
                self.as_mut().set_auto_newline(
                    crate::get_attr(&hostname.to_string(), "auto_newline")
                        .map(|s| s == "1")
                        .unwrap_or(false),
                );
                &crate::get_attr(&hostname.to_string(), "content").map(|s| {
                    self.as_mut().set_content(s.as_str().into());
                });
                self.set_status_label("Successful".into());
            } else {
                self.set_status_label("Error occur!".into());
            }
        }

        #[qinvokable]
        pub fn set_bool(&self, attr: &QString, value: bool) {
            let v = if value { 1 } else { 0 }.to_string();
            crate::set_attr(self.hostname(), &attr.to_string(), &v).ok();
        }
        #[qinvokable]
        pub fn set_lcd_content(&self, content: &QString) {
            crate::set_attr(self.hostname(), "content", &content.to_string()).ok();
        }
    }
}
