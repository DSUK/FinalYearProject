#pragma once
#include<functional>
template <typename T>
class CLibraryContainer
{
	T _item;
	std::function<void(T)> _destroy;
	bool alive = true;
public:

	template<typename T> CLibraryContainer(T item, std::function<void(T)> destroy) {
		_item = item;
		_destroy = destroy;
	}
	~CLibraryContainer() {
		if(alive) _destroy(_item);
	}
	T operator &() const {
		return _item;
	}
	void destroy() {
		if(alive) _destroy(_item);
		alive = false;
	}
};
