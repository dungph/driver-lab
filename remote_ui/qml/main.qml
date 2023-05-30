// qml/main.qml

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.12

// This must match the qml_uri and qml_version
// specified with the #[cxx_qt::qobject] macro in Rust.
import io.demo 1.0

ApplicationWindow { 
    width: 570 
    height: 400
    visible: true
    title: "LCD 1602 Controller"

    Demo {
        id: demo
        loggedIn: false
        statusLabel: "You may need to setup your ssh key for the root account on the host"
        content: "Content"
        blink: false
        cursor: false
        scroll: false
        display: false
        backlight: false
        autoNewline: false
    }

    ColumnLayout {
        anchors.fill: parent
        Label {
            text: demo.statusLabel
            font.pixelSize: 16 
            color: "green"

        }
        RowLayout {
            id: topRow
            Layout.fillWidth: true

            TextField {
                id: hostnameInput
                Layout.fillWidth: true
                enabled: !demo.loggedIn
                font.pixelSize: 16 
                placeholderText: "Hostname"
            }

            Button {
                id: loginButton
                text: "Login"
                font.pixelSize: 16 
                visible: !demo.loggedIn
                onClicked: demo.tryHostname(hostnameInput.text)
            }
            
            Button {
                id: logoutButton
                text: "Logout"
                font.pixelSize: 16 
                visible: demo.loggedIn
                onClicked: demo.setLoggedIn(false)
            }
        }

        ScrollView {
            Layout.fillWidth: true
            RowLayout {
                id: middleRow
                Layout.fillWidth: true

                CheckBox {
                    id: blinkCheckbox
                    text: "Blink"
                    font.pixelSize: 16 
                    enabled: demo.loggedIn
                    checked: demo.blink
                    onClicked: demo.setBool("blink", blinkCheckbox.checked)
                }

                CheckBox {
                    id: cursorCheckbox
                    text: "Cursor"
                    font.pixelSize: 16 
                    enabled: demo.loggedIn
                    checked: demo.cursor
                    onClicked: demo.setBool("cursor", cursorCheckbox.checked)
                }

                CheckBox {
                    id: displayCheckbox
                    text: "Display"
                    font.pixelSize: 16 
                    enabled: demo.loggedIn
                    checked: demo.display
                    onClicked: demo.setBool("display", displayCheckbox.checked)
                }

                CheckBox {
                    id: backlightCheckbox
                    text: "Backlight"
                    font.pixelSize: 16 
                    enabled: demo.loggedIn
                    checked: demo.backlight
                    onClicked: demo.setBool("backlight", backlightCheckbox.checked)
                }

                CheckBox {
                    id: scrollCheckbox
                    text: "Scroll"
                    font.pixelSize: 16 
                    enabled: demo.loggedIn
                    checked: demo.scroll
                    onClicked: demo.setBool("scroll", scrollCheckbox.checked)
                }
                
                CheckBox {
                    id: autoNewlineCheckbox
                    text: "Auto newline"
                    font.pixelSize: 16 
                    enabled: demo.loggedIn
                    checked: demo.autoNewline
                    onClicked: demo.setBool("auto_newline", autoNewlineCheckbox.checked)
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            border.color: "grey"

            ScrollView {
                id: contentView
                anchors.fill: parent

                TextArea {
                    id: contentInput
                    placeholderText: "Content"
                    text: demo.content
                    font.pixelSize: 16
                    enabled: demo.loggedIn
                }
            }
        }

        Button {
            Layout.fillWidth: true
            id: submitButton
            enabled: demo.loggedIn
            text: "Submit"
            font.pixelSize: 16 
            Layout.alignment: Qt.AlignRight
            onClicked: demo.setLcdContent(contentInput.text)
        }
    }
}
