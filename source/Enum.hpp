#pragma once

#include <initializer_list>
#include <type_traits>

template <class TEnum>
struct EnumFlags {
	using Enum = TEnum;
	using Underlying = std::underlying_type_t<TEnum>;
    
	Underlying mValue;
    
	EnumFlags()
		: mValue{ 0 }
    {
	}
    
	EnumFlags(TEnum e)
		: mValue{ 1 << static_cast<Underlying>(e) }
    {
	}
    
	bool IsSet(EnumFlags mask) const {
		return (mValue & mask.mValue) == mask.mValue;
	}
    
	bool IsSet(std::initializer_list<TEnum> enums) {
		EnumFlags flags;
		for (auto& e : enums) {
			flags.mValue |= static_cast<Underlying>(e);
		}
		return IsSet(flags);
	}
    
	bool IsSetExclusive(EnumFlags mask) const {
		return mValue == mask.mValue;
	}
    
	bool IsSetExclusive(std::initializer_list<TEnum> enums) {
		EnumFlags flags;
		for (auto& e : enums) {
			flags.mValue |= static_cast<Underlying>(e);
		}
		return IsSetExclusive(flags);
	}
    
	void SetOn(EnumFlags mask) {
		mValue |= mask.mValue;
	}
    
	void SetOff(EnumFlags mask) {
		mValue &= ~mask.mValue;
	}
    
	void Set(EnumFlags mask, bool enabled) {
		if (enabled) {
			SetOn(mask);
		} else {
			SetOff(mask);
		}
	}
    
	EnumFlags& operator|=(EnumFlags that) const {
		mValue |= that.mValue;
		return *this;
	}
    
	EnumFlags& operator&=(EnumFlags that) const {
		mValue &= that.mValue;
		return *this;
	}
    
	EnumFlags& operator^=(EnumFlags that) const {
		mValue ^= that.mValue;
		return *this;
	}
    
	EnumFlags& operator|=(TEnum e) const {
		mValue |= 1 << static_cast<Underlying>(e);
		return *this;
	}
    
	EnumFlags& operator&=(TEnum e) const {
		mValue &= 1 << static_cast<Underlying>(e);
		return *this;
	}
    
	EnumFlags& operator^=(TEnum e) const {
		mValue ^= 1 << static_cast<Underlying>(e);
		return *this;
	}
    
	EnumFlags operator|(EnumFlags that) const { return EnumFlags(mValue | that.mValue); }
	EnumFlags operator&(EnumFlags that) const { return EnumFlags(mValue & that.mValue); }
	EnumFlags operator^(EnumFlags that) const { return EnumFlags(mValue ^ that.mValue); }
    
	EnumFlags operator|(TEnum e) const { return EnumFlags(mValue | 1 << static_cast<Underlying>(e)); }
	EnumFlags operator&(TEnum e) const { return EnumFlags(mValue & 1 << static_cast<Underlying>(e)); }
	EnumFlags operator^(TEnum e) const { return EnumFlags(mValue ^ 1 << static_cast<Underlying>(e)); }
    
	EnumFlags operator~() const { return EnumFlags(~mValue); }
};
