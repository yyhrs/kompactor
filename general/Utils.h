#ifndef UTILS_H
#define UTILS_H

#include <cstdarg>
#include <string>

// General purpose utility functions and classes
namespace Alamo
{

	template<typename T>
	struct range
	{
		T min;
		T max;

		void normalize() {
			// Swap min and max if necessary
			if (min > max)
			{
				T tmp = max;
				max = min;
				min = tmp;
			}
		}

		range(const T&_min, const T&_max) {
			this->min = _min; this->max = _max;
		}
		range() {
		}
	};

}

#endif
