#pragma once
#include "engine/master.h"
#include <variant>
#include <optional>
#include <map>
#include <string>
#include <vector>
#include <memory>

class ADFEntry {
    enum class ADFType {
        map,
        array
    };
    enum class TokenType {
        String,
        StartMap,
        StartArray,
        EndMap,
        EndArray,
        EndFile
    };
    struct Token {
        TokenType type;
        std::optional<std::string> content;
        inline Token() {}
        inline Token (TokenType Type, std::optional<std::string> Content = std::nullopt) { type = Type; content = Content; }
    };

    std::variant<std::map<std::string, ADFEntry>, std::string, std::vector<ADFEntry>> data;
    std::shared_ptr<std::string> Filename = nullptr;


    class Tokenizer {
        std::ifstream filestream;
    public:
        inline Tokenizer(std::string FilePath) {
            filestream = Filesystem::GetFile(FilePath, std::ios::in | std::ios_base::binary);
        }
        Token ReadToken();
    };

    void ADFError(std::string error) const;

    ADFEntry(ADFType Type, Tokenizer& Tokenizer, std::shared_ptr<std::string> filename);
    ADFEntry(std::string content, std::shared_ptr<std::string> filename) { data = std::move(content); filename = filename; }
    ADFEntry() {};
public:
    //! Creates an ADF tree from a .adf file.
    static ENGINEEXPORT ADFEntry FromFile(const std::string& FilePath);

    //! Used for manual adding of string-type entries.
    inline static ADFEntry String(std::string Content = std::string()) { ADFEntry ret; ret.data = Content; return ret; }
    //! Used for manual adding of map-type entries.
    inline static ADFEntry Map(std::map<std::string, ADFEntry> Content = std::map<std::string, ADFEntry>()) { ADFEntry ret; ret.data = Content; return ret; }
    //! Used for manual adding of array-type entries.
    inline static ADFEntry Array(std::vector<ADFEntry> Content = std::vector<ADFEntry>()) { ADFEntry ret; ret.data = Content; return ret; }

    inline bool IsString() {
        return std::holds_alternative<std::string>(data);
    }
    inline bool IsMap() {
        return std::holds_alternative<std::map<std::string, ADFEntry>>(data);
    }
    inline bool IsArray() {
        return std::holds_alternative<std::vector<ADFEntry>>(data);
    }
    inline std::string& GetString() {
        if (!IsString()) {
            ADFError("Tried to get a string value from a different type of an ADF entry!");
        }
        return std::get<std::string>(data);
    }
    inline std::map<std::string, ADFEntry>& GetChildren() {
        if (!IsMap()) {
            ADFError("Tried to get a list of children from a different type of an ADF entry!");
        }
        return std::get<std::map<std::string, ADFEntry>>(data);
    }
    inline std::vector<ADFEntry>& GetArray() {
        if (!IsArray()) {
            ADFError("Tried to get an array from a different type of an ADF entry!");
        }
        return std::get<std::vector<ADFEntry>>(data);
    }

    inline const std::string& GetString() const {
        if (!std::holds_alternative<std::string>(data)) {
            ADFError("Tried to get a string value from a different type of an ADF entry!");
        }
        return std::get<std::string>(data);
    }
    inline const std::map<std::string, ADFEntry>& GetChildren() const {
        if (!std::holds_alternative<std::map<std::string, ADFEntry>>(data)) {
            ADFError("Tried to get a list of children from a different type of an ADF entry!");
        }
        return std::get<std::map<std::string, ADFEntry>>(data);
    }
    inline const std::vector<ADFEntry>& GetArray() const {
        if (!std::holds_alternative<std::vector<ADFEntry>>(data)) {
            ADFError("Tried to get an array from a different type of an ADF entry!");
        }
        return std::get<std::vector<ADFEntry>>(data);
    }



    inline ADFEntry& operator[](int i) {
        return GetArray()[i];
    }

    inline ADFEntry& operator[](const std::string& name) {
        return GetChildren().at(name);
    }

    inline const ADFEntry& operator[](int i) const {
        return GetArray()[i];
    }

    inline const ADFEntry& operator[](const std::string& name) const {
        return GetChildren().at(name);
    }

    inline bool HasChild(const std::string& name) const {
        return GetChildren().contains(name);
    }

    inline bool HasChildren() const {
        return GetChildren().empty();
    }

    inline bool HasElements() const {
        return GetArray().empty();
    }

};