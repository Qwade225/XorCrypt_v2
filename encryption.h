#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>

std::string xor_encrypt(const std::string &data, const std::string &key);
std::string aes_encrypt(const std::string &data, const std::string &key);
std::string aes_decrypt(const std::string &data, const std::string &key);

void encrypt_file(const std::string &input_path, const std::string &output_path, const std::string &key);

#endif