#pragma once

namespace Resources
{
	class IResource
	{
	public:
		virtual ~IResource() {};

	protected:
		virtual void LoadData(const char* path) = 0;
	};
}
