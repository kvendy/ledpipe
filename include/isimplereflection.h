#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <functional>


class ISimpleReflection;
using Method = std::function<std::string(std::string)>;

class ISimpleReflection
{
	std::unordered_map<std::string, Method> methods;
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
protected:
	void addMethod(const std::string& name, Method method)
	{
		methods[name] = method;
	}

	virtual void reflect() = 0;
};

