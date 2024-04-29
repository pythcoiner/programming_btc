//
// Created by cc1 on 16/12/22.
//
#include "secp256k1.h"
#include <cassert>




void test_fieldelement(){

    // == !=
    FieldElement a = {2,31};
    FieldElement b = {2,31};
    FieldElement c = {15,31};
    FieldElement d = {15,31};


    assert(a == b);
    assert(a != c);
    assert(!(a != b));

    // add
    a = {2,31};
    b = {15,31};

    assert((a + b) == FieldElement(17,31));

    a = {17,31};
    b = {21,31};

    assert((a + b) == FieldElement(7,31));

    a = {29,31};
    b = {4,31};

    // sub
    assert((a - b) == FieldElement(25,31));

    a = {15,31};
    b = {30,31};

    assert((a - b) == FieldElement(16,31));

    //mul
    a = {24,31};
    b = {19,31};

    assert((a * b) == FieldElement(22,31));

    // pow
    a = {17,31};

    assert((a.pow(3)) == FieldElement(15,31));

    a = {5,31};
    b = {18,31};

    assert((a.pow(5) * b) == FieldElement(16,31));

    //div
    a = {3,31};
    b = {24,31};
    assert((a / b) == FieldElement(4,31));

    a = {17,31};
    assert((a.pow(-3)) == FieldElement(29,31));

    a = {4,31};
    b = {11,31};
    assert((a.pow(-4) * b) == FieldElement(13,31));

    std::cout << "test_field_element passed!" << std::endl;

}

void test_point() {

    uint256_t prime = 223;

    FieldElement a = {0, prime};
    FieldElement b = {7, prime};

    // point on curve
    FieldElement x = {192, prime};
    FieldElement y = {105, prime};
    Point p = {x, y, a, b};

    x = {17, prime};
    y = {56, prime};
    p = {x, y, a, b};

    x = {17, prime};
    y = {56, prime};
    p = {x, y, a, b};

    x = {1, prime};
    y = {193, prime};
    p = {x, y, a, b};

}

void test_point_not_on_curve(){
    uint256_t prime = 223;

    FieldElement a = {0, prime};
    FieldElement b = {7, prime};

    // point on curve
    FieldElement x = {192, prime};
    FieldElement y = {105, prime};
    Point p = {x, y, a, b};
    // point not on curve
    try{
        x = {200, prime};
        y = {119, prime};
        p = {x, y, a, b};

        throw 0;
    }

    catch(int i){
        if ( i == -1) i = -2;
        if ( i == 0) {
            std::cerr << "point_not_on_curve fail!" << std::endl;
            throw "point_not_on_curve fail!"; }

    }

    try{
        x = {42, prime};
        y = {99, prime};
        p = {x, y, a, b};

        throw 0;
    }

    catch(int i){
        if ( i == -1) i = -2;
        if ( i == 0) {
            std::cerr << "point_not_on_curve fail!" << std::endl;
            throw "point_not_on_curve fail!"; }

    }

    //addition


    std::cout << "test_point passed!" << std::endl;


}

void test_point_add(){

    uint256_t prime = 223;
    FieldElement a ={0, prime};
    FieldElement b ={7, prime};

    std::vector<std::vector<uint256_t>> v;
    v.push_back(std::vector<uint256_t>{192, 105, 17, 56, 170, 142});
    v.push_back(std::vector<uint256_t>{47, 71, 117, 141, 60, 139});
    v.push_back(std::vector<uint256_t>{143, 98, 76, 66, 47, 71});

    FieldElement x1 = {};
    FieldElement y1= {};

    FieldElement x2= {};
    FieldElement y2= {};

    FieldElement x3= {};
    FieldElement y3= {};

    FieldElement infinity = {0,prime};
    infinity.set_infinity();

    Point p1 = {a,b};
    Point p2 = {a,b};
    Point p3 = {a,b};

    for (auto i: v){

        x1 = {i[0], prime} ;
        y1 = {i[1], prime} ;
        p1 = {x1,y1,a,b};

        x2 = {i[2], prime} ;
        y2 = {i[3], prime} ;
        p2 = {x2,y2,a,b};

        x3 = {i[4], prime} ;
        y3 = {i[5], prime} ;
        p3 = {x3,y3,a,b};

        assert( p1 + p2 == p3);

    }
    std::cout << "test_point_add passed!" << std::endl;

}

void test_point_mult(){

    uint256_t prime = 223;
    FieldElement a ={0, prime};
    FieldElement b ={7, prime};

    std::vector<std::vector<uint256_t>> v;
    v.push_back(std::vector<uint256_t>{2,192,105,49,71});
    v.push_back(std::vector<uint256_t>{2,143,98,64,168});
    v.push_back(std::vector<uint256_t>{2, 47, 71, 36, 111});
    v.push_back(std::vector<uint256_t>{4, 47, 71, 194, 51});
    v.push_back(std::vector<uint256_t>{8, 47, 71, 116, 55});
    v.push_back(std::vector<uint256_t>{21, 47, 71,0,0});

    FieldElement x1 = {};
    FieldElement y1= {};
    FieldElement x2= {};
    FieldElement y2= {};
    FieldElement infinity = {0,prime};
    infinity.set_infinity();

    Point p1 = {a,b};
    Point p2 = {a,b};

    for (auto i: v){
//        std::cout << i[0] << "-" << i[1] << "-" << i[2] << "-" << i[3] << "-" << i[4] << "-" << std::endl;
        uint256_t s = i[0];

        x1 = {i[1], prime} ;
        y1 = {i[2], prime} ;
        p1 = {x1,y1,a,b};

        if (i[3] != 0){
            x2 = {i[3], prime} ;
            y2 = {i[4], prime} ;
            p2 = {x2,y2,a,b};
        }
        else p2 = {infinity,infinity,a,b};


        assert((p1 * s == p2));

    }
    std::cout << "test_point_mult passed!" << std::endl;

}

void test_s256point(){

    S256Field xg = {uint256_t("0x79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798")};
    S256Field yg = {uint256_t("0x483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8")};

    auto P = uint256_t("0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f");
    auto N = uint256_t("0xfffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141");

    S256Point G = {xg, yg};

    Point p ;

    p = S256Point(S256Field(uint256_t("0x5cbdf0646e5db4eaa398f365f2ea7a0e3d419b7e0330e39ce92bddedcac4f9bc")), S256Field(uint256_t("0x6aebca40ba255960a3178d6d861a54dba813d0b813fde7b5a5082628087264da")));
    assert(G * 7 == p );


    p = S256Point(S256Field(uint256_t("0xc982196a7466fbbbb0e27a940b6af926c1a74d5ad07128c82824a11b5398afda")), S256Field(uint256_t("0x7a91f9eae64438afb9ce6448a1c133db2d8fb9254e4546b6f001637d50901f55")));
    assert(G * 1485 == p );

    uint256_t m = uint256_t(std::pow(2,128));
    p = S256Point(S256Field(uint256_t("0x8f68b9d2f63b5f339239c1ad981f162ee88c5678723ea3351b7b444c9ec4c0da")), S256Field(uint256_t("0x662a9f2dba063986de1d90c2b6be215dbbea2cfe95510bfdf23cbf79501fff82")));
    assert(G * m == p );

    std::cout << "test_s256point passed!" << std::endl;

}

void test_pbk(){

    S256Field xg = {uint256_t("0x79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798")};
    S256Field yg = {uint256_t("0x483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8")};
    S256Point G = {xg, yg};

    S256Point p = S256Point(G * 997002999);
    string cpr = "039d5ca49670cbe4c3bfa84c96a8c87df086c6ea6a24ba6b809c9de234496808d5";
    string ucpr = "049d5ca49670cbe4c3bfa84c96a8c87df086c6ea6a24ba6b809c9de234496808d56fa15cc7f3d38cda98dee2419f415b7513dde1301f8643cd9245aea7f3f911f9";
    assert(p.compressed() == cpr);
    assert(p.uncompressed() == ucpr);

    p = G * 123;
    cpr = "03a598a8030da6d86c6bc7f2f5144ea549d28211ea58faa70ebf4c1e665c1fe9b5";
    ucpr = "04a598a8030da6d86c6bc7f2f5144ea549d28211ea58faa70ebf4c1e665c1fe9b5204b5d6f84822c307e4b4a7140737aec23fc63b65b35f86a10026dbd2d864e6b";
    assert(p.compressed() == cpr);
    assert(p.uncompressed() == ucpr);

    p = G * 42424242;
    cpr = "03aee2e7d843f7430097859e2bc603abcc3274ff8169c1a469fee0f20614066f8e";
    ucpr = "04aee2e7d843f7430097859e2bc603abcc3274ff8169c1a469fee0f20614066f8e21ec53f40efac47ac1c5211b2123527e0e9b57ede790c4da1e72c91fb7da54a3";
    assert(p.compressed() == cpr);
    assert(p.uncompressed() == ucpr);

    std::cout << "test_pbk passed!" << std::endl;


}

int test_main(){
    test_fieldelement();
    test_point();
    test_point_not_on_curve();
    test_point_add();
    test_point_mult();
    test_s256point();
    test_pbk();


    std::cout << "All test passed successfully!" << std::endl;

//    std::cout << picosha2::hash256_hex_string(string("test ")) << std::endl;
    return 0;
}


