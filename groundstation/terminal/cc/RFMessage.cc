#include "../header/RFMessage.h"

RFMessage::RFMessage() {

}

RFMessage::RFMessage(std::string contents) {
    this->contents_=contents;
}

std::string RFMessage::getContents() const {
    return this->contents_;
}
