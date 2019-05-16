#pragma once
#include <map>
#include <vector>
#include <string>
#include <functional>

using Method = std::function<std::string(std::string)>;

class ISimpleReflection
{
	std::map<std::string, Method> methods;
	std::map<std::string, std::string> descriptions;
public:
	ISimpleReflection() {};
	virtual ~ISimpleReflection() {};

	std::string call(const std::string& name, std::string parameters)
	{
		//return (methods[name])(parameters);

		auto it = methods.find(name);

		if (it == methods.end())
			return "FAILED\n";
		else
			return it->second(parameters);
	}

	std::map<std::string, std::string> listMethods()
	{
		return descriptions;
	}

protected:
	void addMethod(const std::string& name, const std::string& description, Method method)
	{
		methods[name] = method;
		descriptions[name] = description;
	}

	virtual void reflect() = 0;
};

