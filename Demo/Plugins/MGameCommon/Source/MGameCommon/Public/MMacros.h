// ================================================================================================
// UObject 单例类
#ifndef M_UOBJECT_SINGLETON_DEFINE
#define M_UOBJECT_SINGLETON_DEFINE(TYPE) \
private: \
static TYPE* Singleton; \
static bool bHasShutdown; \
public: \
static TYPE* Get(); \
virtual void BeginDestroy() override;	
#endif

#ifndef M_UOBJECT_SINGLETON_IMPL
#define M_UOBJECT_SINGLETON_IMPL(TYPE) \
TYPE* TYPE::Singleton = nullptr; \
bool TYPE::bHasShutdown = false; \
TYPE* TYPE::Get() \
{ \
if (!Singleton && !bHasShutdown) \
{ \
Singleton = NewObject<TYPE>(); \
Singleton->AddToRoot(); \
} \
return Singleton; \
}  \
void TYPE::BeginDestroy() \
{ \
if (Singleton == this) \
{ \
bHasShutdown = true; \
Singleton = nullptr; \
} \
Super::BeginDestroy(); \
}
#endif

//
// ================================================================================================
