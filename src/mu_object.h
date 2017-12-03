#pragma once
#include <memory>
#include <string>
#define MU_ACCESS(x)
#define MU_SIGNALS public
#define MU_SLOTS MU_ACCESS(mu_slot)
#define MU_EMIT

# define SLOT(a)     "1"#a

# define SIGNAL(a)   "2"#a

class mu_event;

class mu_event_dispatcher
{
public:
	virtual void post_event(mu_event* pEvent) = 0;
	void handle_event(mu_event* pEvent);
};

class mu_type
{

};

enum ConnectionType 
{
	AutoConnection,
	DirectConnection,
	QueuedConnection,
	BlockingQueuedConnection,
	UniqueConnection = 0x80
};
class mu_object;

struct mu_metaobject
{
	class Connection;
	enum Call
	{
		InvokeMetaMethod,
		AsyncInvokeMetaMethod,
		IndexOfMethod
	};
	static void activate(mu_object *sender, int signal_index, void **argv);
	static void activate(mu_object *sender, const mu_metaobject *, int local_signal_index, void **argv);
	static void activate(mu_object *sender, int signal_offset, int local_signal_index, void **argv);

	struct
	{
		const mu_metaobject *superdata;
		const std::string* stringdata;
		const unsigned int *data;
		typedef void(*StaticMetacallFunction)(mu_object*, mu_metaobject::Call, int, void **);
		typedef mu_type*(*StaticMetaTypeFunction)(int, void **);
		StaticMetacallFunction static_metacall;
		StaticMetaTypeFunction static_metatype;
		void *extradata; //reserved for future use
	} d;
};

#define MU_OBJECT \
public: \
	static const mu_metaobject staticMetaObject; \
	virtual const mu_metaobject *metaObject() const; \
private:\
	static void mu_static_metacall(mu_object *, mu_metaobject::Call, int, void **); \
	static mu_type* mu_static_metatype(int _id, void **_a); \
struct QPrivateSignal {};

class mu_object
{
	MU_OBJECT
public:
	mu_object();
	virtual ~mu_object();
MU_SIGNALS:
	void destroyed(mu_object * obj= 0);
	void objectNameChanged(const std::string &objectName);
public:
	static void mu_object::connect(mu_object *sender, const char *signal,
		const mu_object *receiver, const char *method,
		ConnectionType type = ConnectionType::AutoConnection);
	void mu_object::connect(const char *signal,
		const mu_object *receiver, const char *method,
		ConnectionType type = ConnectionType::AutoConnection);
	static void set_dispatcher(mu_event_dispatcher* pDispatcher);
	void* get_p();
private:
	void* pPrivate;
};

