#include <node.h>
#include <v8.h>
#include <DzCot/DzCot.h>

using namespace node;
using namespace v8;

struct ParamWrapper
{
    Isolate *isolate;
    Persistent<Function> jsFunc;
};

CotEntry firstCotEntry(intptr_t context)
{
    ParamWrapper *wrapper = (ParamWrapper *)context;
    auto runningContext = wrapper->isolate->GetCurrentContext();
    auto nullVal = Null(wrapper->isolate);
    Local<Value> argv[0];
    auto result = Local<Function>::New(wrapper->isolate, wrapper->jsFunc)->Call(runningContext, nullVal, 0, argv);
    wrapper->jsFunc.Reset();
}

CotEntry jsCotEntry(intptr_t context)
{
    ParamWrapper *wrapper = (ParamWrapper *)context;
    auto runningContext = wrapper->isolate->GetCurrentContext();
    auto nullVal = Null(wrapper->isolate);
    Local<Value> argv[0];
    auto result = Local<Function>::New(wrapper->isolate, wrapper->jsFunc)->Call(runningContext, nullVal, 0, argv);
    delete wrapper;
}

void throwError(Isolate *isolate, const char *message)
{
    auto errorMsg = String::NewFromUtf8(isolate, message).ToLocalChecked();
    isolate->ThrowException(Exception::TypeError(errorMsg));
}

void dzRunHosts(const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = args.GetIsolate();
    int argsCount = args.Length();
    if (argsCount < 7 || argsCount > 8)
    {
        return throwError(isolate, "Wrong number of arguments");
    }
    if (!args[0]->IsInt32() || args[0].As<Number>()->Value() <= 0)
    {
        return throwError(isolate, "Parameter 'smallStackSize' need to be a positive integer");
    }
    int smallStackSize = int(args[0].As<Number>()->Value());
    if (!args[1]->IsInt32() || args[1].As<Number>()->Value() <= 0)
    {
        return throwError(isolate, "Parameter 'middleStackSize' need to be a positive integer");
    }
    int middleStackSize = int(args[1].As<Number>()->Value());
    if (!args[2]->IsInt32() || args[2].As<Number>()->Value() <= 0)
    {
        return throwError(isolate, "Parameter 'largeStackSize' need to be a positive integer");
    }
    int largeStackSize = int(args[2].As<Number>()->Value());
    if (!args[3]->IsInt32() || args[3].As<Number>()->Value() <= 0)
    {
        return throwError(isolate, "Parameter 'lowestPri' need to be a positive integer");
    }
    int lowestPri = int(args[3].As<Number>()->Value());
    if (!args[4]->IsInt32() || args[4].As<Number>()->Value() <= 0)
    {
        return throwError(isolate, "Parameter 'dftPri' need to be a positive integer");
    }
    int dftPri = int(args[4].As<Number>()->Value());
    if (!args[5]->IsInt32() || args[5].As<Number>()->Value() <= 0)
    {
        return throwError(isolate, "Parameter 'dftSType' need to be a positive integer");
    }
    int dftSType = int(args[5].As<Number>()->Value());
    ParamWrapper wrapper;
    wrapper.isolate = isolate;
    wrapper.jsFunc.Reset(isolate, Local<Function>::Cast(args[6]));
    int result = 0;
    result = DzRunHosts(
        1,
        smallStackSize,
        middleStackSize,
        largeStackSize,
        lowestPri,
        dftPri,
        dftSType,
        firstCotEntry,
        intptr_t(&wrapper));
    args.GetReturnValue().Set(result);
}

void dzStartCot()
{
}

void Method(const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = args.GetIsolate();
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, "world").ToLocalChecked());
}

void Initialize(Local<Object> exports)
{
    NODE_SET_METHOD(exports, "hello", Method);
    NODE_SET_METHOD(exports, "dzRunHost", dzRunHosts);
    // NODE_SET_METHOD(exports, "dzStartCot", dzStartCot);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)
