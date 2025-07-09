#include "gui_interface.h"
#include "encryption.h"

#include <gtk/gtk.h>
#include <glib.h>
#include <string>
#include <fstream>
#include <iostream>

// Reusable function to get key from user securely
std::string prompt_for_key(const std::string& title) {
    GtkWidget *key_dialog = gtk_dialog_new_with_buttons(
        title.c_str(), NULL, GTK_DIALOG_MODAL,
        "_OK", GTK_RESPONSE_OK,
        "_Cancel", GTK_RESPONSE_CANCEL, NULL);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(key_dialog));
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(entry), FALSE); // Hide characters
    gtk_container_add(GTK_CONTAINER(content_area), entry);
    gtk_widget_show_all(key_dialog);

    std::string key;
    if (gtk_dialog_run(GTK_DIALOG(key_dialog)) == GTK_RESPONSE_OK) {
        const char *key_text = gtk_entry_get_text(GTK_ENTRY(entry));
        key = key_text;
    }
    gtk_widget_destroy(key_dialog);
    return key;
}

// Encrypt File callback
void on_encrypt_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Choose a file to encrypt",
                                                    NULL,
                                                    GTK_FILE_CHOOSER_ACTION_OPEN,
                                                    "_Cancel", GTK_RESPONSE_CANCEL,
                                                    "_Open", GTK_RESPONSE_ACCEPT,
                                                    NULL);
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filepath = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        std::string key = prompt_for_key("Enter Encryption Key");

        if (!key.empty()) {
            std::string input_file = filepath;
            std::string output_file = input_file + ".enc";
            encrypt_file(input_file, output_file, key);

            GtkWidget *done_dialog = gtk_message_dialog_new(NULL,
                                                            GTK_DIALOG_DESTROY_WITH_PARENT,
                                                            GTK_MESSAGE_INFO,
                                                            GTK_BUTTONS_OK,
                                                            "File encrypted and saved as:\n%s",
                                                            output_file.c_str());
            gtk_dialog_run(GTK_DIALOG(done_dialog));
            gtk_widget_destroy(done_dialog);
        }

        g_free(filepath);
    }

    gtk_widget_destroy(dialog);
}

// Decrypt File callback
void on_decrypt_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Choose a file to decrypt",
                                                    NULL,
                                                    GTK_FILE_CHOOSER_ACTION_OPEN,
                                                    "_Cancel", GTK_RESPONSE_CANCEL,
                                                    "_Open", GTK_RESPONSE_ACCEPT,
                                                    NULL);
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filepath = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        std::string key = prompt_for_key("Enter Decryption Key");

        if (!key.empty()) {
            std::string input_file = filepath;
            std::string output_file = input_file + ".dec"; // Save decrypted version
            encrypt_file(input_file, output_file, key); // XOR is symmetric

            GtkWidget *done_dialog = gtk_message_dialog_new(NULL,
                                                            GTK_DIALOG_DESTROY_WITH_PARENT,
                                                            GTK_MESSAGE_INFO,
                                                            GTK_BUTTONS_OK,
                                                            "File decrypted and saved as:\n%s",
                                                            output_file.c_str());
            gtk_dialog_run(GTK_DIALOG(done_dialog));
            gtk_widget_destroy(done_dialog);
        }

        g_free(filepath);
    }

    gtk_widget_destroy(dialog);
}

// GUI Launcher
void launch_gui() {
    gtk_init(nullptr, nullptr);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "XorCrypt v2.0");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *label = gtk_label_new("Welcome to XorCrypt - Secure and Stateless");
    gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);

    GtkWidget *encrypt_button = gtk_button_new_with_label("Encrypt File");
    GtkWidget *decrypt_button = gtk_button_new_with_label("Decrypt File");
    gtk_box_pack_start(GTK_BOX(vbox), encrypt_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), decrypt_button, TRUE, TRUE, 0);

    g_signal_connect(encrypt_button, "clicked", G_CALLBACK(on_encrypt_button_clicked), NULL);
    g_signal_connect(decrypt_button, "clicked", G_CALLBACK(on_decrypt_button_clicked), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();
}