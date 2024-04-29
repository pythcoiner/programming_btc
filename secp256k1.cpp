//
// Created by cc1 on 15/12/22.
//
#include "secp256k1.h"
#include "hash.h"

FieldElement::FieldElement() {
    this->num = 1;
    this->prime =0;
    this->infinity = true;
}

FieldElement::FieldElement(const uint256_t& num, const uint256_t& prime) {
    try{
        if (num >= prime) throw string("num not in field range:(");
        this->num = num;
        this->prime = prime;
        this->infinity = false;
    }
    catch(string const& e){std::cout << e << num << ", " << prime << ")" << std::endl;}
}

FieldElement::~FieldElement() = default;

string FieldElement::str() const {
//    string out = "FieldElement_" + boost::lexical_cast<string>(prime) + "{" + boost::lexical_cast<string>(num) + "}";
    string out = "FieldElement{" + boost::lexical_cast<string>(num) + "}";
    return out;
}

bool FieldElement::operator==(const FieldElement &other) const {
    if (this->prime == other.prime) {
        if (this->num == other.num) return true;
        if (this->is_infinity() and other.is_infinity()) return true;
    }
    return false;

}

bool FieldElement::operator!=(const FieldElement &other) const {
    if (*this == other) return false;
    else return true;

}

FieldElement FieldElement::operator+(const FieldElement &other) const {
    try{
        if (this->prime != other.prime) throw string("primes not match");
        // cast to uin1024 for avoid overflow on * operation
        uint1024_t _num = (uint1024_t(this->num) + uint1024_t(other.num))% uint1024_t(this->prime);
        return {uint256_t(_num), this->prime};
    }

    catch(string const& e) { std::cerr << e ;}
    return {};
}

FieldElement FieldElement::operator-(const FieldElement &other) const {
    try{
        // cast to uin1024 for avoid overflow on * operation
        if (this->prime != other.prime) throw string("primes not match!");
        bmp::int1024_t a,b,c;
        a = bmp::int1024_t(this->num);
        b = bmp::int1024_t(other.num);
        c = bmp::int1024_t(this->prime);

        bmp::int1024_t n = (a - b)%c;
        if (n<0) n = n + this->prime;

        auto out = uint256_t(n);

        return {out, this->prime};
    }
    catch(string const& e){std::cerr << e; return {};}

}

FieldElement FieldElement::operator*(const FieldElement &other) const {
//    std::cout << "  FieldElement::operator*(other.num =" << other.num.str() << ")" << std::endl;
//    std::cout << "  this->num =" << this->num.str() << std::endl;
//    std::cout << "  this->prime =" << this->prime.str() << std::endl;
    try{
        //cast to uin1024 for avoid overflow on * operation
        if (this->prime != other.prime) throw string("primes not match!");

//        std::cout << "-123-" << std::endl;
        auto temp = uint1024_t(uint1024_t(this->num) * uint1024_t(other.num));
//        std::cout << temp << std::endl;
        temp = temp % uint1024_t(this->prime);
//        std::cout << temp << std::endl;
//        std::cout << "-456-" << std::endl;

        uint1024_t _num = (uint1024_t(this->num) * uint1024_t(other.num))% uint1024_t(this->prime);
//        std::cout << "  _num =" << _num.str() << std::endl;
        auto _num2 = uint256_t(_num);
//        std::cout << "  return _num2 =" << _num2.str() << std::endl;
//        std::cout << "  --- end-other"  << std::endl;
        return {_num2, this->prime};
    }
    catch(string const& e){std::cerr << e;return {};}

}

FieldElement FieldElement::operator*(const boost::multiprecision::uint256_t &coefficient) const {
//    std::cout << "  FieldElement::operator*(coeff =" << coefficient.str() << ")" << std::endl;
//    std::cout << "  this->num= =" << this->num.str() << ")" << std::endl;
//    std::cout << "  this->prime= =" << this->prime.str() << ")" << std::endl;
    // cast to uin1024 for avoid overflow on * operation
    uint1024_t _num = (uint1024_t(this->num) * uint1024_t(coefficient)) % uint1024_t(this->prime);
//    std::cout << "  _num =" << _num.str() << std::endl;
    auto _num2 = uint256_t(_num);
//    std::cout << "  return _num2 =" << _num2.str() << std::endl;
//    std::cout << "  --- end-coef"  << std::endl;
    return {_num2, this->prime};
}

FieldElement FieldElement::pow(const int512_t &exponent) const {
    // cast to uin1024 for avoid overflow on pow operation
    auto exp = bmp::int1024_t(exponent);
    while (exp < 0){
        exp += uint1024_t(this->prime) - uint1024_t(1);
    }
    uint1024_t _num = bmp::powm(uint1024_t(this->num),uint1024_t(exp),uint1024_t(this->prime));
    return {uint256_t(_num), this->prime};
}

FieldElement FieldElement::operator/(const FieldElement &other) const {
    try{
            if (this->prime != other.prime) throw string("primes not match!");
            uint1024_t _num = ((uint1024_t(this->num) * bmp::powm(uint1024_t(other.num), uint1024_t(this->prime) - 2,
                                                                            uint1024_t(this->prime)))% uint1024_t(this->prime));
            return {uint256_t(_num), this->prime};
    }
    catch(string const& e){std::cerr << e;return {};}
}

bool FieldElement::is_infinity() const {
    return this->infinity & this->num == this->prime +1;
}

void FieldElement::set_infinity(bool b) {
    this->infinity = b;
    if (b) this->num = this->prime +1;
}

void FieldElement::set_infinity() {
    this->set_infinity(true);
}


S256Field::S256Field(const boost::multiprecision::uint256_t &num) {
    try{
        if (num >= P) throw string("num not in field range:(");
        this->num = num;
        this->prime = P;
        this->infinity = false;
    }
    catch(string const& e){std::cout << e << num << ", " << prime << ")" << std::endl;}

}

string S256Field::str() const {
    string out = boost::lexical_cast<string>(num);
    return out;
}


Point::Point(const FieldElement& x, const FieldElement& y, const FieldElement& a, const FieldElement& b) {
    this->x = x;
    this->y = y;
    this->a = a;
    this->b = b;

    if (x.is_infinity() and y.is_infinity()) return;

    // check the point is on the curve
    try{
        if ((y*y) != (x*x*x) + (a * x) + b) throw string("except");
    }
    catch (string){
        std::cout << "Point not on curve! => " ;
        std::cout << this->str() << std::endl;
        throw -1;
     }

}

Point::Point(const FieldElement& a, const FieldElement& b) {

    this->x = {0,a.prime};
    this->y = {1,a.prime};
    this->a = a;
    this->b = b;

}

Point::Point()= default;

bool Point::operator==(const Point &other) const {

    // point have no curve param
    if (this->a.is_infinity() or this->b.is_infinity()) return false;
    if (other.a.is_infinity() or other.b.is_infinity()) return false;

    if(this->x == other.x and this->y == other.y and this->a == other.a and this->b == other.b )
        return true;
    else return false;
}

bool Point::operator!=(const Point &other) const {
    // point have no curve param
    if (this->a.is_infinity() or this->b.is_infinity()) return true;
    if (other.a.is_infinity() or other.b.is_infinity()) return true;
    if (*this == other) return false;
    else return true;
}

string Point::str() const {
    string out = "Point(x=" + this->x.str() + ", y="
            + this->y.str() + ") [a="
            + this->a.str() + ") , b="
            + this->b.str() + "]" ;
    return out;
}

Point Point::operator+(const Point &other) {

    // point have no curve param
    if (this->a.is_infinity() or this->b.is_infinity()) return {};
    if (other.a.is_infinity() or other.b.is_infinity()) return {};

    FieldElement inf = {0, this->x.prime};
    inf.set_infinity();

    try{
        // case 0.0
        if (this->a != other.a or this->b != other.b) throw string("a or b not match!");

        //case 0.1
        if (this->x.is_infinity()) return other;
        if (other.x.is_infinity()) return *this;

        //case 1
        if (this->x == other.x and this->y != other.y) {
//            std::cout << "case1" << std::endl;
            return {inf, inf, this->a, this->b};
        }

        //case2
        if (this->x != other.x){
//            std::cout << "case2" << std::endl;
            FieldElement _s = (other.y - this->y) / (other.x - this->x);
            FieldElement _x = _s.pow(2) - this-> x - other.x;
            FieldElement _y = _s * (this->x - _x) - this->y;
            return {_x, _y, this->a, this->b};
        }

        //case3
        if (*this == other){
//            std::cout << "case3" << std::endl;
//            std::cout << "  ( ((this->x * this->x)*3 )+ this->a)=" << ( ((this->x * this->x)*3 )+ this->a).str() << std::endl;
//            std::cout << "  (this->y * 2)=" << (this->y * 2).str() << std::endl;
//            FieldElement _s = ( (this->x.pow(2) * 3) + this->a) / (this->y * 2);
            FieldElement _s = ( ((this->x * this->x)*3 )+ this->a) / (this->y * 2);
//            std::cout << "  _s=" << _s.str() << std::endl;
            FieldElement _x = _s.pow(2) -  (this->x * 2);
//            std::cout << "  _x=" << _x.str() << std::endl;
            FieldElement _y = _s * (this->x - _x) - this->y;
//            std::cout << "  _y=" << _y.str() << std::endl;
            auto out = Point(_x, _y, this->a, this->b);
            return out;
        }

        //case4
        if (*this == other and this->y == this->x * 0) {
//            std::cout << "case4" << std::endl;
            return {inf, inf, this->a, this->b};
        }

    }
    catch(string const& e){std::cerr << e;return {};}

}

Point Point::operator*(const int512_t &coefficient) {


    if (this->a.is_infinity() or this->b.is_infinity()) return *this;

    int512_t coef = coefficient;

    Point current = *this;
    FieldElement _x = {0, this->a.prime};
    _x.set_infinity();
    FieldElement _y = {0, this->a.prime};
    _y.set_infinity();

    Point result = {_x, _y, this->a, this->b};

    while(coef){
        if (coef & 1) {
            result = result + current ;
//            std::cout << "result = result + current =" << result.str() << std::endl;
        }
        current = current + current;
        coef = coef >> 1;

    }
    return result;

}


S256Point::S256Point() = default;

S256Point::S256Point(const S256Field &x, const S256Field &y) {
    this->x = x;
    this->y = y;
    this->a = S256Field(A);
    this->b = S256Field(B);

    if (x.is_infinity() and y.is_infinity()) return;

    // check the point is on the curve
    try{
        if ((y*y) != (x*x*x) + (a * x) + b) throw string("except");
    }
    catch (string){
        std::cout << "S256Point(const S256Field &x, const S256Field &y) => Point not on curve! => " ;
        std::cout << this->str() << std::endl;
        throw -1;
    }


}

S256Point::S256Point(Point p) {
    this->x = p.x;
    this->y = p.y;
    this->a = p.a;
    this->b = p.b;

}

Point S256Point::operator*(const boost::multiprecision::int512_t &coefficient) {

    int512_t coef = coefficient % N;
    return Point::operator*(coef);
}

string S256Point::compressed() {
    if (this->y.num % 2 == 0) {
        std::stringstream ss;
        ss << "02" << std::hex << this->x.num;
        return ss.str();
    }
    else {
        std::stringstream ss;
        ss << "03" << std::hex << this->x.num;
        return ss.str();
    }
}

string S256Point::uncompressed() {
    std::stringstream ss;
    ss << "04" << std::hex << this->x.num << std::hex << this->y.num;
    return  ss.str();
}

S256Point S256Point::operator=(S256Point p) {
    this->x = p.x;
    this->y = p.y;
    this->a = p.a;
    this->b = p.b;
    return *this;
}


Key::Key(string pv) {

    this->k = uint256_t(pv);
    Point _p = G * this->k;

    this->p = S256Point(_p);

}

string Key::pb_compressed() {
    return this->p.compressed();
}

string Key::pb_uncompressed() {
    return this->p.uncompressed();
}

Key Key::from_uncompressed(string ucp) {
    if (ucp.length() == 130){
        string x = "0x" + ucp.substr(2,64);
        string y = "0x" + ucp.substr(66,64);

        return Key(S256Field(uint256_t(x)), S256Field(uint256_t(y)));

    }
    else{
        std::cerr << "uncompressed key length might be 130 char lenght in hex format!!!" << std::endl;
    }

    return Key("");
}

Key::Key(S256Field x, S256Field y) {
    p = S256Point(x, y);
}

string Key::pb_compressed_hash() {
    return hash160(this->pb_compressed());
}

string Key::pb_uncompressed_hash() {
    return hash160(this->pb_uncompressed());
}

Key::Key() {

}

uint256_t Key::get_k() {
    return this->k;
}

void Key::incr() {
    this->k++;
    this->p = this->p + this->G;

}
