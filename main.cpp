
#include "secp256k1.h"
#include <chrono>
#include "test_main.h"
//#include "hash.h"

AVLTree* load_pub_key(){
    // Load public keys
    auto tree = new AVLTree();

    //open file
    std::ifstream file("pubkey");

    if (!file.is_open()){
        std::cerr << "Cannot open pubkey file!" << std::endl;
        return nullptr;
    }
    else
    {
        string line;
        string strt;
        Key k;
        std::list<string> lst;

        int count = 0;

        while(getline(file, line)){
            strt = line.substr(0,2);
            line = line.substr(2);
            uint256_t index;
            // if uncompressed format
            if (strt == "04"){
                line = "0x" + line;
                // get the compressed format
                line = Key::from_uncompressed(line).pb_compressed();
            }
                // handle wrong format
            else if (strt != "02" && strt != "03"){
                std::cout << "invalid key: " << line << std::endl;
            }

            // insert into AVLTree structure
            line = "0x" + line;
            index = uint256_t(line);
            tree->insert(index, uint64_t(0));
            count++;


        }
        std::cout << "successfully load " << count << " pubkeys" << std::endl;
        file.close();
        return tree;

    }

}

AVLTree* load_pub_key_hash(string path){
    // Load public keys hash
    auto tree = new AVLTree();

    //open file
    std::ifstream file(path);

    if (!file.is_open()){
        std::cerr << "Cannot open pubkeyhash file!" << std::endl;
        return nullptr;
    }
    else
    {
        string line;
        string strt;
        Key k;
        std::list<string> lst;

        int count = 0;

        while(getline(file, line)){
            strt = line.substr(0,2);
            line = line.substr(2);
            uint256_t index;

            // insert into AVLTree structure
            line = "0x" + line;
            index = uint256_t(line);
            tree->insert(index, uint64_t(0));
            count++;

        }
        std::cout << "successfully load " << count << " pubkeys hash" << std::endl;
        return tree;

    }

}

uint256_t gen_random(){

    boost::mt19937 rng(std::time(nullptr));
    boost::uniform_int<uint256_t> gen(0, uint256_t("0xfffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141"));

    return gen(rng);
}

int main(){



    test_main();

    AVLTree* pubkey = load_pub_key();
    if (pubkey == nullptr) std::cerr << "unable to load pubkey tree" << std::endl;

    AVLTree* pubkeyhash = load_pub_key_hash("pubkeyhash");
    if (pubkeyhash == nullptr) std::cerr << "unable to load pubkeyhash tree" << std::endl;

    Key pvk;
    uint256_t pbk, pbkh, pbkh2;

    size_t count = 0;
    size_t last_count = 0;
    time_t start = std::time(nullptr);
    time_t actual;

//    uint256_t seed = 0xfffffffffffffffffff;
    while (true){
//        seed++;
        auto seed = gen_random();

        pvk = Key("0x" +seed.str());

//        std::cout << "-----------start of for" << std::endl;
        for (int i = 1; i < 10000; i++ )
        {
            pvk.incr();

            pbk = uint256_t("0x" + pvk.pb_compressed());
            pbkh = uint256_t("0x" + pvk.pb_uncompressed_hash());
            pbkh2 = uint256_t("0x" + pvk.pb_compressed_hash());

            if (pubkey->search(pbk) != nullptr || pubkeyhash->search(pbkh) != nullptr
                || pubkeyhash->search(pbkh2) != nullptr) {
                std::cout << "found result for: " << pvk.get_k() << std::endl;

                auto output = std::ofstream("found", std::ios::app);

                if (output) {
                    output << seed.str() << std::endl;
                    output.close();

                }

                return 0;
            }

            count++;

            if (count % 10000 == 0) {
                actual = std::time(nullptr);
                std::cout << "Total:" << count << " - " << (count - last_count) << " in " << (actual - start) << "sec."
                          << std::endl;
                start = actual;
                last_count = count;
            }
//            std::cout << pbk << std::endl;
        }



    }








    return 0;
}
