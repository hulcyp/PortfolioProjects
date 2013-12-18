#pragma once
#include "Job.h"

namespace Monky
{
	class HashBufferJob : public Job
	{
	public:
		HashBufferJob( const char* buffer, int size, const std::string& eventToFire, float priority = 1.0f );

		virtual void execute();
		virtual void fireCompletedEvent();

	private:
		const char* m_buffer;
		int m_size;
		unsigned int m_hash;
	};
}