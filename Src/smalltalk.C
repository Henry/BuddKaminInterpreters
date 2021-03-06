#include <iostream>

#include "lisp.h"
#include "environment.h"

extern Env globalEnvironment;
extern Env commands;
extern Env valueOps;
extern List emptyList;

// isTrue is not used, but still needs to be defined
int isTrue(Expression* cond)
{
    return 0;
}

//
//      objects are subclasses of functions, since their main job is
//      to respond to a message, which is expressed in a function notation
//      objects maintain their own methods and data
//

/// SmalltalkObject
class Object
:
    public Function
{
    Env methods;
    Env data;

    friend class SubclassMethod;

public:

    Object(Environment* m, Environment* d)
    :
        methods(m),
        data(d)
    {}

    virtual ~Object()
    {
        methods = 0;
        data = 0;
    }

    virtual void print()
    {
        std::cout<< "<object>";
    }

    virtual void apply(Expr&, ListNode*, Environment*);

    // methods used by classes to create new instances
    // note these are invoked only on classes, not simple instances
    ListNode* getNames();
    Environment* getMethods();
};

//
//      methods are also like functions, and thus subclass UserFunction
//
class Method
:
    public UserFunction
{
protected:
    Method():UserFunction(0, 0, 0)
    {}

public:
    Method(ListNode* anames, Expression* bod)
    :
        UserFunction(anames, bod, 0)
    {}

    virtual void doMethod
    (
        Expr&,
        Object*,
        ListNode*,
        Environment*,
        Environment*
    );

    virtual Method* isMethod()
    {
        return this;
    }
};
///- SmalltalkObject

/// SmalltalkObjectApply
void Object::apply(Expr& target, ListNode* args, Environment* rho)
{
    // need at least a message
    if (args->length() < 1)
    {
        target = error("ill formed message expression");
        return;
    }

    Symbol* message = args->head()->isSymbol();
    if (!message)
    {
        target = error("object needs message");
        return;
    }

    // now see if message is a method
    Environment* meths = methods;
    Expression* methexpr = meths->lookup(*message);
    Method* meth = 0;
    if (methexpr)
    {
        meth = methexpr->isMethod();
    }

    if (!meth)
    {
        target = error("unrecognized method name: ", message->name());
        return;
    }

    // now just execute the method (take off message from arg list)
    meth->doMethod(target, this, args->tail(), data, rho);
}

void Method::doMethod
(
    Expr& target,
    Object* self,
    ListNode* args,
    Environment* ctx,
    Environment* rho
)
{
    // Change the exectution context
    context_ = ctx;

    // put self on the front of the argument list
    List newargs(new ListNode(self, args));

    // and execute the function
    apply(target, newargs, rho);

    // clean up arg list
    newargs = 0;
}
///- SmalltalkObjectApply

/// SmalltalkObjectGetNames
ListNode* Object::getNames()
{
    Environment* datavals = data;
    Expression* x = datavals->lookup(Symbol("names"));
    if ((!x) || (!x->isList()))
    {
        error("impossible case in Object::getNames");
        return 0;
    }
    return x->isList();
}
///- SmalltalkObjectGetNames

Environment* Object::getMethods()
{
    // note that getMethods is used only on classes

    Environment* datavals = data;
    Expression* x = datavals->lookup(Symbol("methods"));
    if ((!x) || (!x->isEnvironment()))
    {
        error("impossible case in Object::getMethods");
        return 0;
    }
    return x->isEnvironment();
}

//
//      Integers are make into objects as well
//

static Env IntegerMethods;

/// SmalltalkInteger
class IntegerObject
:
    public Object
{
    Expr value;

public:

    IntegerObject(int v):Object(IntegerMethods, 0)
    {
        value = new IntegerExpression(v);
    }

    virtual ~IntegerObject()
    {
        value = 0;
    }

    virtual void print()
    {
        if (value())
        {
            value()->print();
        }
    }

    virtual IntegerExpression* isInteger()
    {
        return value()->isInteger();
    }
};
///- SmalltalkInteger

class IntegerBinaryMethod
:
    public Method
{
    int (*fun) (int, int);

public:

    IntegerBinaryMethod(int (*thefun) (int, int))
    {
        fun = thefun;
    }

    virtual void doMethod
    (
        Expr&,
        Object*,
        ListNode*,
        Environment*,
        Environment*
    );
};

void IntegerBinaryMethod::doMethod
(
    Expr& target,
    Object* self,
    ListNode* args,
    Environment* ctx,
    Environment* rho
)
{
    if (args->length() != 1)
    {
        target = error("wrong number of args passed to int op");
        return;
    }
    IntegerExpression* left = self->isInteger();
    IntegerExpression* right = args->head()->isInteger();
    if ((!left) || !right)
    {
        target = error("int op with non integers");
        return;
    }
    target = new IntegerObject(fun(left->val(), right->val()));
}

// smalltalk symbols just evaluate to themselves
/// SmalltalkSymbol
class SmalltalkSymbol
:
    public Symbol
{
public:

    SmalltalkSymbol(const Symbol& sym)
    :
        Symbol(sym)
    {}

    virtual void eval(Expr& target, Environment*, Environment*)
    {
        target = this;
    }
};
///- SmalltalkSymbol

class IfMethod
:
    public Method
{

public:

    virtual void doMethod
    (
        Expr&,
        Object*,
        ListNode*,
        Environment*,
        Environment*
    );
};

/// SmalltalkIfMethod
void IfMethod::doMethod
(
    Expr& target,
    Object* self,
    ListNode* args,
    Environment* ctx,
    Environment* rho
)
{
    if (args->length() != 2)
    {
        target = error("wrong number of args for if");
        return;
    }
    IntegerExpression* cond = self->isInteger();
    if (!cond)
    {
        target = error("impossible!", "no cond in if");
        return;
    }
    if (cond->val())
    {
        args->at(0)->eval(target, valueOps, rho);
    }
    else
    {
        args->at(1)->eval(target, valueOps, rho);
    }
}
///- SmalltalkIfMethod

//
//      the smalltalk reader handles symbols and integers
//

class SmalltalkReader
:
    public ReaderClass
{
protected:
    virtual Expression* readExpression();
};

/// SmalltalkReader
Expression* SmalltalkReader::readExpression()
{
    // See if it's an integer
    if (isdigit(*p_))
    {
        return new IntegerObject(readInteger());
    }

    // Might be a signed integer
    if ((*p_ == '-') && isdigit(*(p_ + 1)))
    {
        p_++;
        return new IntegerObject(-readInteger());
    }

    // Or it might be a symbol
    if (*p_ == '#')
    {
        const char* symbolStart = p_;
        int nSymbolChars = 0;

        while (!isSeparator(*p_))
        {
            nSymbolChars++;
            p_++;
        }

        return new SmalltalkSymbol
        (
            Symbol(std::string(symbolStart, nSymbolChars))
        );
    }

    // Anything else, do as before
    return ReaderClass::readExpression();
}
///- SmalltalkReader

//
//      method new is used to create a new object
//

class NewMethod
:
    public Method
{
public:
    virtual void doMethod
    (
        Expr&,
        Object*,
        ListNode*,
        Environment*,
        Environment*
    );
};

/// SmalltalkNewMethodDoMethod
void NewMethod::doMethod
(
    Expr& target,
    Object* self,
    ListNode* args,
    Environment* ctx,
    Environment* rho
)
{
    // get the list of instance names
    ListNode* names = self->getNames();

    // cdr down the list, making a list of values (initially zero)
    ListNode* values = emptyList;
    for (ListNode* p = names; !p->isNil(); p = p->tail())
    {
        values = new ListNode(new IntegerExpression(0), values);
    }

    // make the new environment for the names
    Environment* newenv = new Environment(names, values, rho);

    // make the new object
    target = new Object(self->getMethods(), newenv);
}
///- SmalltalkNewMethodDoMethod

//
//      method Subclass used to create a new class
//
class SubclassMethod
:
    public Method
{
public:
    virtual void doMethod
    (
        Expr&,
        Object*,
        ListNode*,
        Environment*,
        Environment*
    );
};

/// SmalltalkSubclassMethod
void SubclassMethod::doMethod
(
    Expr& target,
    Object* self,
    ListNode* args,
    Environment* ctx,
    Environment* rho
)
{
    // the argument list is added to the list of variables
    ListNode* vars = self->getNames();
    while (!args->isNil())
    {
        vars = new ListNode(args->head(), vars);
        args = args->tail();
    }

    // the method table is empty, but points to inherited method table
    Environment* newmeth
    (
        new Environment(emptyList, emptyList, self->getMethods())
    );

    // make the new data area
    Environment* newEnv = new Environment(emptyList, emptyList, rho);
    newEnv->add(new Symbol("names"), vars);
    newEnv->add(new Symbol("methods"), newmeth);

    // now make the new object
    Environment* meths = self->methods;
    target = new Object(meths, newEnv);
}
///- SmalltalkSubclassMethod

//
//      method Method used to create new methods
//

class MethodMethod
:
    public Method
{
public:
    virtual void doMethod
    (
        Expr&,
        Object*,
        ListNode*,
        Environment*,
        Environment*
    );
};

/// SmalltalkMethodMethodDoMethod
void MethodMethod::doMethod
(
    Expr& target,
    Object* self,
    ListNode* args,
    Environment* ctx,
    Environment* rho
)
{
    if (args->length() != 3)
    {
        target = error("method definition requires three arguments");
        return;
    }
    Symbol* name = args->at(0)->isSymbol();
    if (!name)
    {
        target = error("method definition missing name");
        return;
    }

    ListNode* argNames = args->at(1)->isList();
    if (!argNames)
    {
        target = error("method definition missing arg names");
        return;
    }
    // put self on front of arg names
    argNames = new ListNode(new Symbol("self"), argNames);

    // get the method table for the given class
    Environment* methTable = self->getMethods();

    // put method in place
    methTable->add(name, new Method(argNames, args->at(2)));

    // yield as value the name of the function
    target = name;
}
///- SmalltalkMethodMethodDoMethod

/// SmalltalkInitialize
ReaderClass* initialize()
{
    // initialize global variables
    ReaderClass* reader = new SmalltalkReader;

    // the only commands are the assignment command and begin
    Environment* vo = valueOps;
    vo->add(new Symbol("set"), new SetStatement);
    vo->add(new Symbol("begin"), new BeginStatement);

    // initialize the global environment
    Environment* ge = globalEnvironment;

    // first create the object ``Object''
    Environment* objMethods = new Environment(emptyList, emptyList, 0);
    Environment* objClassMethods = new Environment(emptyList, emptyList,
    objMethods);
    objClassMethods->add(new Symbol("new"), new NewMethod);
    objClassMethods->add(new Symbol("subclass"), new SubclassMethod);
    objClassMethods->add(new Symbol("method"), new MethodMethod);
    Environment* objData = new Environment(emptyList, emptyList, 0);
    objData->add(new Symbol("names"), emptyList());
    objData->add(new Symbol("methods"), objMethods);
    ge->add(new Symbol("Object"), new Object(objClassMethods, objData));

    // now make the integer methods
    IntegerMethods = new Environment(emptyList, emptyList, objMethods);
    Environment* im = IntegerMethods;
    // the integer methods are just as before
    im->add(new Symbol("+"), new IntegerBinaryMethod(PlusFunction));
    im->add(new Symbol("-"), new IntegerBinaryMethod(MinusFunction));
    im->add(new Symbol("*"), new IntegerBinaryMethod(TimesFunction));
    im->add(new Symbol("/"), new IntegerBinaryMethod(DivideFunction));
    im->add(new Symbol("="), new IntegerBinaryMethod(IntEqualFunction));
    im->add(new Symbol("<"), new IntegerBinaryMethod(LessThanFunction));
    im->add(new Symbol(">"), new IntegerBinaryMethod(GreaterThanFunction));
    im->add(new Symbol("if"), new IfMethod);
    ge->add(new Symbol("Integer"), new Object(objClassMethods, objData));

    return reader;
}
///- SmalltalkInitialize
