#pragma once

namespace cbs
{
	class IndexMaker
	{
	private:
		int index;
		int maximum;

	public:
		template <typename T> inline int make(T value);

		inline IndexMaker()
		{
			index = 0;
			maximum = 1;
		}
		template <typename T>
		inline IndexMaker(T value)
		{
			index = make(value);
			maximum = StateConst<T>::count;
		}
		template <typename T>
		inline IndexMaker& operator |=(T value)
		{
			typedef StateConst<T> C;
			index += make(value) * maximum;
			maximum *= C::count;
			return *this;
		}
		template <typename T>
		inline const IndexMaker operator |(T value) const
		{
			IndexMaker out = *this;
			out |= value;
			return out;
		}
		template <typename T>
		inline friend const IndexMaker operator |(T value, const IndexMaker im)
		{
			im |= value;
			return im;
		}

		inline operator int() const
		{
			return index;
		}
	};

	template <typename T> inline int IndexMaker::make(T value)
	{
		typedef StateConst<T> C;
		int res = value - C::offset;
		assert(res < C::count);
		return res;
	}
	template <> inline int IndexMaker::make<bool>(bool value)
	{
		return value ? 1 : 0;
	}

}