#include "engine/Filesystem.h"
#include "engine/ADF.h"
#include <cctype>

ADFObject::Token ADFObject::Tokenizer::ReadToken() {
    char currchar;
    std::string TokenContent;

    // Skip any whitespace.
    do {
        currchar = filestream.get();
    } while (std::isspace(currchar));

    // Any of the other cases
    switch (currchar) {
    case '\"':
        TokenContent.reserve(256);

        currchar = filestream.get(); // Has to be like this as to not include the starting quotation mark.
        while (!(currchar == '\"' || currchar == -1)) {
            TokenContent.push_back(currchar);
            currchar = filestream.get();
        }

        TokenContent.shrink_to_fit();

        return Token(TokenType::String, TokenContent);
    case '{':
        return Token(TokenType::StartObject);
    case '[':
        return Token(TokenType::StartArray);
    case '}':
    case ']':
    case -1:
        return Token(TokenType::End);
    }

    // Unquoted string
    if (std::isgraph(currchar)) {
        TokenContent.reserve(256);

        do {
            TokenContent.push_back(currchar);
            currchar = filestream.get();
        } while (std::isgraph(currchar));

        TokenContent.shrink_to_fit();

        return Token(TokenType::String, TokenContent);
    }

    // Warning silencer.
    return Token();
}

ADFObject::ADFObject(ADFType Type, Tokenizer& Tokenizer) {
    switch (Type) {
    
    case ADFType::map: {
        data = std::map<std::string, ADFObject>();
        std::map<std::string, ADFObject>& mapdata = std::get<std::map<std::string, ADFObject>>(data);

        while (true) {
            Token Token = Tokenizer.ReadToken();
            std::string key;

            switch (Token.type) {
            case TokenType::String:
                key = std::move(Token.content.value());
                break;
            case TokenType::StartObject:
            case TokenType::StartArray:
                Engine::Error("An ADF entry cannot be a key!");
            case TokenType::End:
                return;
            }

            Token = Tokenizer.ReadToken();

            switch (Token.type) {
                case TokenType::String:
                    mapdata.emplace(std::move(key), ADFObject(Token.content.value()));
                break;
                case TokenType::StartObject:
                    mapdata.emplace(std::move(key), ADFObject(ADFType::map, Tokenizer));
                break;
                case TokenType::StartArray:
                    mapdata.emplace(std::move(key), ADFObject(ADFType::array, Tokenizer));
                break;
                case TokenType::End:
                    Engine::Error("Incomplete ADF key/value pair!");
            }
        }

        return;
    }
    case ADFType::array: {
        data = std::vector<ADFObject>();
        std::vector<ADFObject>& arraydata = std::get<std::vector<ADFObject>>(data);

        while (true) {
            Token Token = Tokenizer.ReadToken();

            switch (Token.type) {
                case TokenType::String:
                    arraydata.emplace_back(ADFObject(Token.content.value()));
                break;
                case TokenType::StartObject:
                    arraydata.emplace_back(ADFObject(ADFType::map, Tokenizer));
                break;
                case TokenType::StartArray:
                    arraydata.emplace_back(ADFObject(ADFType::array, Tokenizer));
                break;
                case TokenType::End:
                    return;
            }
        }

        return;
    }
    
    }
}

ENGINEEXPORT ADFObject ADFObject::FromFile(std::string FilePath) {
    Tokenizer Tokenizer(FilePath);
    return ADFObject(ADFType::map, Tokenizer);
}
