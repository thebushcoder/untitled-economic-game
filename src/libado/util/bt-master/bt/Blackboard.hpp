#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <SFML/System.hpp>

namespace bt
{

class Blackboard
{
public:
    void SetBool(std::string key, bool value) { bools[key] = value; }
    bool GetBool(std::string key)
    {
        if (bools.find(key) == bools.end()) {
            bools[key] = false;
        }
        return bools[key];
    }
    bool HasBool(std::string key) const { return bools.find(key) != bools.end(); }
    void RemoveBool(std::string key){bools.erase(key);}

    void SetInt(std::string key, int value)  { ints[key] = value; }
    int GetInt(std::string key)
    {
        if (ints.find(key) == ints.end()) {
            ints[key] = -1;
        }
        return ints[key];
    }
    bool HasInt(std::string key) const  { return ints.find(key) != ints.end(); }
    void RemoveInt(std::string key){ints.erase(key);}

    void SetFloat(std::string key, float value)  { floats[key] = value; }
    float GetFloat(std::string key)
    {
        if (floats.find(key) == floats.end()) {
            floats[key] = -1.0f;
        }
        return floats[key];
    }
    bool HasFloat(std::string key) const  { return floats.find(key) != floats.end(); }
    void RemoveFloat(std::string key){floats.erase(key);}

    void SetDouble(std::string key, double value)  { doubles[key] = value; }
    double GetDouble(std::string key)
    {
        if (doubles.find(key) == doubles.end()) {
            doubles[key] = -1.0f;
        }
        return doubles[key];
    }
    bool HasDouble(std::string key) const  { return doubles.find(key) != doubles.end(); }
    void RemoveDouble(std::string key){doubles.erase(key);}

    void SetString(std::string key, std::string value)  { strings[key] = value; }
    std::string GetString(std::string key)
    {
        if (strings.find(key) == strings.end()) {
            strings[key] = "";
        }
        return strings[key];
    }
    bool HasString(std::string key) const  { return strings.find(key) != strings.end(); }
    void RemoveString(std::string key){strings.erase(key);}

    void SetVector(std::string key, sf::Vector2i value)  { vectors[key] = value; }
    sf::Vector2i& GetVector(std::string key)
	{
		if (vectors.find(key) == vectors.end()) {
			vectors[key] = sf::Vector2i();
		}
		return vectors[key];
	}
	bool HasVector(std::string key) const  { return vectors.find(key) != vectors.end(); }
	void RemoveVector(std::string key){vectors.erase(key);}

	void AddAction(std::string key, std::string val){
		if(!HasBool("hasAction")){
			SetBool("hasAction", true);

		}else if(GetBool("hasAction")){
			SetBool("hasAction", true);

		}else{

		}
	}

    using Ptr = std::shared_ptr<Blackboard>;

protected:
    std::unordered_map<std::string, bool> bools;
    std::unordered_map<std::string, int> ints;
    std::unordered_map<std::string, float> floats;
    std::unordered_map<std::string, double> doubles;
    std::unordered_map<std::string, std::string> strings;
    std::unordered_map<std::string, sf::Vector2i> vectors;
};

}
