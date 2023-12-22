#ifndef SHA256_HPP
#define SHA256_HPP

#include <stdint.h>

#include <string>
#include <vector>

namespace ly {

class Sha256
{
public:
    Sha256() {}

    virtual ~Sha256() {}

    bool encrypt(const std::vector<uint8_t>& message, 
                 std::vector<uint8_t>* _digest);

    std::string getHexMessageDigest(const std::string& message);

protected:

    inline uint32_t ch(uint32_t x, uint32_t y, uint32_t z) const;
    inline uint32_t maj(uint32_t x, uint32_t y, uint32_t z) const;
    inline uint32_t big_sigma0(uint32_t x) const;
    inline uint32_t big_sigma1(uint32_t x) const;
    inline uint32_t small_sigma0(uint32_t x) const;
    inline uint32_t small_sigma1(uint32_t x) const;

    bool preprocessing(std::vector<uint8_t>* _message) const;

    bool breakTextInto64ByteChunks(const std::vector<uint8_t>& message, 
                                   std::vector<std::vector<uint8_t>>* _chunks) const;

    bool structureWords(const std::vector<uint8_t>& chunk, 
                        std::vector<uint32_t>* _words) const;

    bool transform(const std::vector<uint32_t>& words, 
                   std::vector<uint32_t>* _message_digest) const;

    bool produceFinalHashValue(const std::vector<uint32_t>& input,
                               std::vector<uint8_t>* _output) const;


private:
    static std::vector<uint32_t> initial_message_digest_;
    static std::vector<uint32_t> add_constant_;
};

inline uint32_t Sha256::ch(uint32_t x, uint32_t y, uint32_t z) const
{
    return (x & y) ^ ((~x) & z);
}

inline uint32_t Sha256::maj(uint32_t x, uint32_t y, uint32_t z) const
{
    return (x & y) ^ (x & z) ^ (y & z);
}

inline uint32_t Sha256::big_sigma0(uint32_t x) const
{
    return (x >> 2 | x << 30) ^ (x >> 13 | x << 19) ^ (x >> 22 | x << 10);
}

inline uint32_t Sha256::big_sigma1(uint32_t x) const
{
    return (x >> 6 | x << 26) ^ (x >> 11 | x << 21) ^ (x >> 25 | x << 7);
}

inline uint32_t Sha256::small_sigma0(uint32_t x) const
{
    return (x >> 7 | x << 25) ^ (x >> 18 | x << 14) ^ (x >> 3);
}

inline uint32_t Sha256::small_sigma1(uint32_t x) const
{
    return (x >> 17 | x << 15) ^ (x >> 19 | x << 13) ^ (x >> 10);
}

} // namespace ly

#endif // SHA256_HPP
