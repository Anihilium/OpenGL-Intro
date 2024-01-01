#pragma once
#include <Resources/IResource.hpp>
#include <unordered_map>
#include <iostream>

namespace Resources
{
	class ResourceManager
	{
	private:
		std::unordered_map<std::string, IResource*> mResourceMap;

	public:
		template<typename T>
		T* Create(const std::string rsrcName, const char* path)
		{
			T* rsc = new T(path);
			mResourceMap[rsrcName] = (IResource*)rsc;
			return rsc;
		}

		template<typename T>
		T* Get(const std::string rsrcName)
		{
			T* rsrc = mResourceMap[rsrcName];
			if (rsrc)
				return rsrc;
			else
				return nullptr;
		}

		void Delete(const std::string rsrcName);

		void Clear();
	};
}