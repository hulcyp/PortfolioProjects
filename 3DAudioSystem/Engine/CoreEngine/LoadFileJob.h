#pragma once
#include "Job.h"

namespace Monky
{
	class LoadFileJob : public Job
	{
	public:
		LoadFileJob( const std::string& filePath, const std::string& eventToFire, float priority = 1.0f );

		virtual void execute();
		virtual void fireCompletedEvent();

	private:
		std::string m_filePath;
		char* m_buffer;
		int m_bufferSize;
	};
}