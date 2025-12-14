#include <cassert>
#include <string>
#include <print>
#include <openssl/evp.h>

using std::println;
using std::string;

string md5(const string& content) {
  EVP_MD_CTX* context = EVP_MD_CTX_new();
  const EVP_MD* md = EVP_md5();
  unsigned char md_value[EVP_MAX_MD_SIZE];
  unsigned int md_len;
  string output;

  EVP_DigestInit_ex2(context, md, NULL);
  EVP_DigestUpdate(context, content.c_str(), content.length());
  EVP_DigestFinal_ex(context, md_value, &md_len);
  EVP_MD_CTX_free(context);

  output.resize(md_len * 2);
  for (unsigned int i = 0 ; i < md_len ; ++i)
    std::sprintf(&output[i * 2], "%02x", md_value[i]);
  return output;
}

int main(void) {
    string test_str = "iwrupvqb";

    long i=1;
    for (;;) {
        string suffix = std::to_string(i);
        string string_to_hash = test_str + suffix;
        string hashed_str = md5(string_to_hash);
        if (hashed_str.starts_with("00000")) break;
        i += 1;
    }
    long part1 = i;

    for (;;) {
        string suffix = std::to_string(i);
        string string_to_hash = test_str + suffix;
        string hashed_str = md5(string_to_hash);
        if (hashed_str.starts_with("000000")) break;
        i += 1;
    }
    long part2 = i;

    println("Part 1: {}", part1);
    println("Part 2: {}", part2);

    return 0;
}

