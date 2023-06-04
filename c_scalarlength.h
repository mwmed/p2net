#pragma once
#include "includes.h"





namespace p2net {
	namespace core {
		namespace data {
			class c_scalarlength {
			public:
				inline c_scalarlength(){}

				inline c_scalarlength(uint64_t length)
				{
					scalar_length = length;

					fix_index();
				}

				uint8_t scalar_index = 0;
				uint64_t scalar_length = 0;



				void fix_index();
			};
		}
	}
}