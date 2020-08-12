#include <token.hpp>

int Token::get_tag(void) const {
    return (int) tag_;
}

void Token::set_tag(int new_tag) {
    tag_ = new_tag;
}