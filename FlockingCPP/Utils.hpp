#pragma once

#include <vector>
#include <memory>

namespace Utils {

	template<class T> 
	class UniqueVector 
	{

	private:

		std::vector<std::unique_ptr<T>> master;


	public:

		UniqueVector() {}

		UniqueVector(int size) : master(std::vector<std::unique_ptr<T>>(size)) {}

		std::vector<std::unique_ptr<T>>* getMaster()
		{
			return &master;
		}

		std::vector<const T*> getAllEntries() 
		{
			std::vector<const T*> entries;

			for (auto const& ptr : master) {

				entries.push_back(ptr.get());
			}
			return entries;
		}

		template <class V>
		std::vector<V*> getEntriesOfType()
		{
			std::vector<V*> entries;

			for (auto const& ptr : master)
			{
				V* elm = dynamic_cast<V*>(ptr.get());

				//Add to list elm if of desired type
				if (elm != nullptr) {
					entries.push_back(elm);
				}

			}
			return entries;
		}

	};


}

