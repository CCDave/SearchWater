
#ifndef _OBJECT_
#define _OBJECT_
class _object
{
public:
	STDMETHOD (init)() PURE;
	STDMETHOD (destory)() PURE;
	inline HRESULT is_init_ok(){return is_init ? S_OK:E_FAIL;}
	inline void init_done(){is_init=true;}
	_object() : is_init(false)
	{}
private:
	BOOL is_init;
};
#endif