本系列是笔者学习王希老师的 GAMES104 现代游戏引擎入门课程时抄写 Piccolo 源码的笔记，系列最终目标是王希老师所言“完成 104 后，人手一个小引擎”“最后能做个多人网络对战游戏”  
笔者代码在[此处](https://link.zhihu.com/?target=https%3A//github.com/AmamiyaRenn/MyPiccolo/tree/v2.5.1)

这一节我们将引入反射系统

## 为什么需要反射系统

上一节我们成功读入了默认设置，接下来是读入资产；现在我们的需求是能够读入顶点、索引（未来还有纹理坐标）以及摄像头的一些设置——因为接下来我们要进入 3D 时代

以摄像头信息为例，我们需要读入`rendering.global.json`中的这部分信息

```
  "camera_config": {
    "pose": {
      "position": {
        "x": -5.0,
        "y": 0.0,
        "z": 3.0
      },
      "target": {
        "x": -4.0,
        "y": 0.0,
        "z": 3.0
      },
      "up": {
        "x": 0.0,
        "y": 0.0,
        "z": 1.0
      }
    },
    "z_far": 1000.0,
    "z_near": 0.1,
    "aspect": {
      "x": 1280.0,
      "y": 768.0
    }
  },
```

通过这些信息，我们可以设置我们程序内部的属性，具体来说，就是`camera_config.h`中的对应属性，那应该通过什么方法来实现呢？

一种简单的方法就是直接在 main 里写 cin，直接读到特定的变量中，不过我们很快会意识到这个是有问题的——当资源数量一多以来后，每多一些变量我们就要写对应的代码读入，这很麻烦

有一种解决方法叫反射：我们输入字符串，通过字符串来改变程序内部的数据（比如创造一个类，或者调用类的某个函数如a->call("memberFunc1")），所以我们得有办法获得程序中类的名称（如`CameraConfig`）、类内成员变量（如`m_z_far`）、类成员函数等信息；一旦我们获得了这部分信息，就可以与外界输入进行对应从而赋值

## 什么是反射

反射部分参考[C++ 反射：从设计到实现](https://link.zhihu.com/?target=https%3A//www.bilibili.com/video/BV1yv4y1m77s/%3Fshare_source%3Dcopy_web%26vd_source%3D78fca262a252b90390c3caa57c3e6f1b)，**下面实现反射部分默认已经看过这个视频或实现过任何一个 tiny runtime reflection lib**

-   **反射(reflection)**：顾名思义就是程序的内省，允许我们在外界获得程序内部的信息甚至执行内部的函数，比如客户端向服务器发起一个请求特定数据的字符串，那服务器就要内省自己有没有这个字符串对应的数据并返回；反射分为三类，类对象反射、类成员数据反射与类成员函数反射
-   **类对象反射**：允许通过字符串在运行时创建对象
-   **类成员反射**：允许通过字符串在运行时获得或修改类成员变量
-   **类成员函数反射**：允许通过字符串在运行时执行某个类成员函数

## 着手实现反射吧！

关于 Piccolo 的反射方法，知乎有两篇文章讲的很好，下面我会从尝试实现这个系统（以 Vector2 为例子）

-   [C++反射-Pilot 引擎的反射实现](https://zhuanlan.zhihu.com/p/502729373)
-   [Games104 Pilot 反射系统](https://zhuanlan.zhihu.com/p/493894595)

反射属于核心层，我们在 core 底下创建 meta/reflection

反射需要注册反射信息，注册反射信息的方法在上面给的视频中就是通过`ClassRegister`的构造函数调用`ClassFactory`（这是个单例——全局只需要一个该类的实例）的 register 函数，在 register 函数中通过 std::map 把类的名称的字符串作为 key，而类的元信息（类的构造函数`create_object`作为 value，类成员变量的名称（字符串）类型（字符串）该变量在类中的内存偏置数值`ClassField`），所以第一步我们需要知道 piccolo 用什么来作为`ClassRegister`

### 注册一个类字段！ FieldFunctionTuple

通过 RSFSC，我们发现了其实就在`startEngine`的第一行`Reflection::TypeMetaRegister::metaRegister();`，这里面完成各种需要被注册的类的注册，我们现在只关注 Vector2 的注册，所以我们找到`TypeWrappersRegister::Vector2();`的实现

这个实现就在`vector2.reflection.gen.h`文件中的`TypeWrapperRegister_Vector2`函数，我们现在以注册一个成员变量 x 为焦点

```
FieldFunctionTuple* field_function_tuple_x =
    new FieldFunctionTuple(&TypeFieldReflectionOparator::TypeVector2Operator::set_x,
                           &TypeFieldReflectionOparator::TypeVector2Operator::get_x,
                           &TypeFieldReflectionOparator::TypeVector2Operator::getClassName,
                           &TypeFieldReflectionOparator::TypeVector2Operator::getFieldName_x,
                           &TypeFieldReflectionOparator::TypeVector2Operator::getFieldTypeName_x,
                           &TypeFieldReflectionOparator::TypeVector2Operator::isArray_x);
TypeMetaRegisterinterface ::registerToFieldMap("Vector2", field_function_tuple_x);// 对REGISTER_FIELD_TO_MAP("Vector2", field_function_tuple_x);进行了宏展开
```

其中`registerToFieldMap`函数为

```
void TypeMetaRegisterinterface::registerToFieldMap(const char* name, FieldFunctionTuple* value)
{
    m_field_map.insert(std::make_pair(name, value));
}
static std::multimap<std::string, FieldFunctionTuple*> m_field_map;
```

-   **std::multimap**：也是一种 map，不过允许单 key 对应多 value

到这里我们看明白了，Piccolo 注册成员变量就是把字符串`"Vector2"`作为 key，把与成员变量相关的函数——如`set_x``get_x`所打包成的元组(`std::tuple`)（也就是那六行我们现在还不知道干什么用的东西）——作为 value

之所以要用`multimap`，就是因为同个类（同个 key）会拥有很多字段（很多 tuple）

现在看一下那六行不知道干啥的函数的实现，下面我会把一眼能看出意思的函数加上注释

```
namespace TypeFieldReflectionOparator
class TypeVector2Operator// 反射操作类：封装了外界使用反射信息的函数
static void set_x(void* instance, void* field_value)
{
    static_cast<Vector2*>(instance)->x = *static_cast<float*>(field_value);
}
static void* get_x(void* instance) { return static_cast<void*>(&(static_cast<Vector2*>(instance)->x)); }
// 得到类的名称
static const char* getClassName() { return "Vector2"; }
// 得到字段（成员变量）名称
static const char* getFieldName_x() { return "x"; }
// 得到字段（成员变量）类型的名称
static const char* getFieldTypeName_x() { return "float"; }
static bool  isArray_x() { return false; }
```

现在我们还有三个函数没搞明白，那就先挑软柿子捏`get_x`可以猜出是外界传入一个类的实例(`instance`)，进行指针强转转为了 Vector2 类型，然后通过`->`操作符访问 Vector2 的字段 x，最后强转为通用类型指针`void *`

这样我们看`set_x`也就明白了——把外界的变量的指针进行了解引用，得到的值赋值给 x

`isArray_x`盲猜是判断字段是否是数组，因为我们现在可以发现我们无法显示判断字段是否为数组

现在我们看这段代码就不会害怕了，当然代码记得开抄（array 部分大同小异不会赘述）

```
FieldFunctionTuple* field_function_tuple_x =
    new FieldFunctionTuple(/* 看上面 */);
typedef std::function<void(void*, void*)>      SetFuncion;// typedef 改成 using 更好读也更modern
typedef std::function<void*(void*)>            GetFuncion;
typedef std::function<const char*()>           GetNameFuncion;
typedef std::function<bool()>                  GetBoolFunc;
typedef std::tuple<SetFuncion, GetFuncion, GetNameFuncion, GetNameFuncion, GetNameFuncion, GetBoolFunc>
        FieldFunctionTuple;
```

### 注册类的构造函数！

现在我们知道字段 x 的注册与使用的过程了(y 也是一样的不赘述)，现在遇到个问题——instance 到底怎么被创建（类实例如何被创建）

构造函数也是需要注册的，所以我们回到`TypeWrapperRegister_Vector2`，发现下面疑似有我们想要的东西

```
ClassFunctionTuple* class_function_tuple_Vector2 = new ClassFunctionTuple(
    &TypeFieldReflectionOparator::TypeVector2Operator::getVector2BaseClassReflectionInstanceList,
    &TypeFieldReflectionOparator::TypeVector2Operator::constructorWithJson,
    &TypeFieldReflectionOparator::TypeVector2Operator::writeByName);
TypeMetaRegisterinterface ::registerToClassMap("Vector2", class_function_tuple_Vector2);// 对REGISTER_BASE_CLASS_TO_MAP进行了宏展开
```

同样的，看一看 registerToClassMap 的代码（值得注意的是所有反射相关的东西会放置在`Reflection`命名域中防止污染）

```
void TypeMetaRegisterinterface::registerToClassMap(const char* name, ClassFunctionTuple* value)
{// 如果没注册过这个类的构造函数，就map insert，否则回收外面new的内存
    if (m_class_map.find(name) == m_class_map.end())
        m_class_map.insert(std::make_pair(name, value));
    else
        delete value;
}
```

现在已经很容易明白了，`ClassFunctionTuple`中存了一些与类注册相关的函数，现在我们要看一看这部分的代码

```
static void* constructorWithJson(const Json& json_context)
{
    Vector2* ret_instance = new Vector2;
    Serializer::read(json_context, *ret_instance);
    return ret_instance;
}
static Json writeByName(void* instance) { return Serializer::write(*(Vector2*)instance); }
// base class
static int getVector2BaseClassReflectionInstanceList(ReflectionInstance*& out_list, void* instance)
{
    int count = 0;
    return count;
}
```

这部分代码就比较抽象了，我们先着重关注类的构造部分，也就是`constructorWithJson`；根据这个名字，我猜是一个用 json 文件的数据初始化构造器函数

函数里面构造了一个 Vector2 的实例，并调用了`Serializer::read`读入了 json 文本与实例，最后把实例转为通用指针`void *`格式送出

比较难理解的是中间的 read 函数，不过现在我不打算去深入序列化，所以我们就先不继续研究了

那么现在路堵死了，我们不知道如何继续读代码了。。。。真的是这样吗？

现在我们不妨换一个角度，去看看 Piccolo 是如何使用这套反射系统的

### 获得字段元信息 FieldAccessor

现在我们从如何获得字段元信息（类型名、字段名）的角度，研究 Piccolo 反射

我们上面已经完成了字段元信息的注册，不过注册的方法是把函数封装到元组中，那么一定有一个函数取出里面的函数并调用了函数，具体来说，哪里存在一个东西调用了元组中的获得字段名（第 3 个函数）、获得字段类型名（第 4 个函数）的函数，那么到底是谁呢？

RTFSC 后，我们发现了如下代码

```
FieldAccessor::FieldAccessor(FieldFunctionTuple* functions) : m_functions(functions)
{
    m_field_type_name = k_unknown_type;
    m_field_name      = k_unknown;
    if (m_functions == nullptr)
    {
        return;
    }

    m_field_type_name = (std::get<4>(*m_functions))();
    m_field_name      = (std::get<3>(*m_functions))();
}
```

这里毫无疑问就是真正调用函数的地方了，Piccolo 通过`FieldAccessor`来提供获得字段元信息的接口

再看这个类的`get``set``isArrayType`函数，也就懂什么意思了（正是对应了`get_x`,`set_x``isArray_x`）

比较难理解的是这个函数，里面涉及到了我们现在还不懂的`TypeMeta`类

```
TypeMeta FieldAccessor::getOwnerTypeMeta()
{
    // todo: should check validation
    TypeMeta f_type((std::get<2>(*m_functions))());
    return f_type;
}
```

这个类到底是什么呢？要回答这个问题，我们要先思考一个问题，我们现在如果已经拥有了一个 Vector2 实例，也注册了这个实例，那么改怎么办才能获得字段 x 的值呢

我们得想办法调用`FieldAccessor`的构造函数（要玩里面传入函数元组信息来确定是哪个字段），然后调用`get`方法，不够现在马上遇到了一个问题——我们在哪调用`FieldAccessor`的构造函数？换句话说，FieldAcccessor 在哪`new`？

### 储存一个类的所有字段的元信息 TypeMeta

RTFSC 后，我们发现正是在 TypeMeta 的构造函数中！

```
std::string m_type_name;
std::vector<FieldAccessor, std::allocator<FieldAccessor>> m_fields;// 这里的std::allocator基本不用关心
TypeMeta::TypeMeta(std::string type_name) : m_type_name(type_name)
{
    m_is_valid = false;
    m_fields.clear();

    for(auto fields_iter=m_field_map.equal_range(type_name); fields_iter.first != fields_iter.second; fields_iter.first++)
    {// 改while为for
        FieldAccessor f_field(fields_iter.first->second);
        m_fields.emplace_back(f_field);// 通过emplace_back来取消拷贝与析构f_field的这一步，直接原地构造后添加到vector
        m_is_valid = true;
    }
}
```

这个函数在已注册的类字段中找到`type_name`(比如 Vector2)，遍历对应类的所有字段，并把其字段函数元组为参数调用`FieldAccessor`的构造函数，并构造好的`f_field`加入到向量中——同个类可能有多个以注册字段，所以要用 vector

现在我们就很明确了，只需要`TypeMeta meta("Vector2")`我们就能通过 meta 来调用`get_x`，也就是说，meta 储存了 Vector2 的元信息

### 创造一个能反射的类的实例 ReflectionInstance

我们已经知道了如何获得元信息了，现在创造一个实例来玩一玩吧；不过如何创造实例呢？

上面我们已经涉及到了实例的部分，就是`void *instance`，现在我们的需求是，通过一个类，这个类拥有 Vector2 的元信息与一个实例，这个实例最好是个指针，这样只需要更换指针指向对象我们就可以指向不同的 Vector2 实例，从而对不同 Vector2 实例`get_x`

Piccolo 中的这个类就是`ReflectionInstance`

```
class ReflectionInstance
{
public:
    ReflectionInstance(TypeMeta meta, void* instance) : m_meta(meta), m_instance(instance) {}
    ReflectionInstance() : m_meta(), m_instance(nullptr) {}
    ReflectionInstance& operator=(const ReflectionInstance& dest)
    {
        if (this == &dest)
            return *this;
        m_instance = dest.m_instance;
        m_meta     = dest.m_meta;
        return *this;
    }
    ReflectionInstance& operator=(ReflectionInstance&& dest)
    {
        if (this == &dest)
            return *this;
        m_instance = dest.m_instance;
        m_meta     = dest.m_meta;
        return *this;
    }

    TypeMeta m_meta;
    void*    m_instance;
};
```

这个类有 meta 信息，也有实例指针，通过重载=来重新指向不同类的元信息与实例（当然也可以是同类的不同实例）

看懂这个类后现在回去就可以看懂`TypeMeta`的其他函数了

现在我们封装一个带反射的实例创造宏

```
#define TypeMetaDef(class_name, ptr) \
    Piccolo::Reflection::ReflectionInstance(Piccolo::Reflection::TypeMeta::newMetaFromName(#class_name), (class_name*)ptr)
static TypeMeta newMetaFromName(std::string type_name) { return TypeMeta(type_name); }
```

我们已经基本拥有一个反射系统了，目前只能反射字段

另一个值得注意的就是`ReflectionPtr`类，这个类其实就是保存了类的名称的字符串与某个实例而已，有上面基础后理解起来也简单；同时关于 array 的反射也就不再赘述

### 回顾一下我们已经了解到东西

现在我们大概有了这样一张图  

![](https://pic4.zhimg.com/v2-ceee1bb06e752c877d9eaab8fb04943f_b.png)

Image

要加深对反射系统的理解，可以看看例子；Piccolo 为了让我们快速掌握代码，提供了一个简单的反射与序列化的使用例子，这个例子就在`meta_example.cpp/h`中

[这篇文章](https://zhuanlan.zhihu.com/p/502729373)讲解了一下这个例子，我就不再赘述了

___

上面的那些`get_x`之类的东西其实是可以通过代码生成器自动生成的，不然每需要一个类反射就要写一堆重复的东西也是挺麻烦的，所以我们需要一个代码生成器

不过现在这一篇已经很长了，所以分割放送，下一期，我们将尝试自动生成反射代码

-   另外：[Piccolo 源码解读](https://link.zhihu.com/?target=https%3A//www.bilibili.com/video/BV1zP4y1R7Ys/%3Fshare_source%3Dcopy_web%26vd_source%3D78fca262a252b90390c3caa57c3e6f1b)出了大家快去看啊啊啊啊

> 本文使用 [Zhihu On VSCode](https://zhuanlan.zhihu.com/p/106057556) 创作并发布

本系列是笔者学习王希老师的 GAMES104 现代游戏引擎入门课程时抄写 Piccolo 源码的笔记，系列最终目标是王希老师所言“完成 104 后，人手一个小引擎”“最后能做个多人网络对战游戏”

笔者代码在[此处](https://link.zhihu.com/?target=https%3A//github.com/AmamiyaRenn/MyPiccolo/tree/v2.5.2)

## 想要自动生成 TypeVector2Operator

自己写个`vector2.reflection.gen.h`也还行，不过如果需要更多的类来反射呢，就会很累了，所以我们得想办法自动生成这部分代码

要生成反射信息，得有专门的代码分析工具来检查分析代码——这样才能知道我们有`Vector2`这个字符串

### PiccoloParser

Piccolo 用的代码分析工具是 PiccoloParser，接下来我们要把这个工具引入

这一部分的讲解在[Piccolo 源码解读第一期](https://link.zhihu.com/?target=https%3A//www.bilibili.com/video/BV1zP4y1R7Ys/%3Fshare_source%3Dcopy_web%26vd_source%3D78fca262a252b90390c3caa57c3e6f1b)中涉及到了，如果没看过的可以看看

下面我给出过程

### 引入 Parser 源代码

看过视频就知道，Parser 的源代码就是 engine/source/meta\_parser，所以直接把代码复制过来即可

我们现在主要关注游戏引擎部分，所以暂时不深入这个的源码（之后可能会出文章分析这部分代码），大概明白干啥的就行

![](https://pic4.zhimg.com/v2-479f57224dbfe992fa1647fc136334cf_b.jpg)

Image

具体的思路就是基于`llvm`编译架构，用`clang`作为前端分析句法，然后`parser`文件夹写的代码分析 clang 的输出，最后基于`mustache`来生成代码

### mustache template

mustache 生成代码需要模板，此处我们也简单的把 engine/template 复制即可

此处我们简单看看模板代码，以`commonReflectionFile.mustache`为例

```
#pragma once
{{#include_headfiles}}
#include "{{headfile_name}}"
{{/include_headfiles}}

namespace Piccolo{
    {{#class_defines}}class {{class_name}};
    {{/class_defines}}
namespace Reflection{
{{#class_defines}}namespace TypeFieldReflectionOparator{
    class Type{{class_name}}Operator{
    public:
        static const char* getClassName(){ return "{{class_name}}";}
        static void* constructorWithJson(const Json& json_context){
            {{class_name}}* ret_instance= new {{class_name}};
            Serializer::read(json_context, *ret_instance);
            return ret_instance;
        }
        static Json writeByName(void* instance){
            return Serializer::write(*({{class_name}}*)instance);
        }
        // base class
        static int get{{class_name}}BaseClassReflectionInstanceList(ReflectionInstance* &out_list, void* instance){
            int count = {{class_base_class_size}};
            {{#class_has_base}}out_list = new ReflectionInstance[count];
            for (int i=0;i<count;++i){
            {{#class_base_class_defines}}
               out_list[i] = TypeMetaDef({{class_base_class_name}},static_cast<{{class_name}}*>(instance));
            {{/class_base_class_defines}}
            }{{/class_has_base}}
            return count;
        }
        // fields
        {{#class_field_defines}}static const char* getFieldName_{{class_field_name}}(){ return "{{class_field_name}}";}
        static const char* getFieldTypeName_{{class_field_name}}(){ return "{{{class_field_type}}}";}
        static void set_{{class_field_name}}(void* instance, void* field_value){ static_cast<{{class_name}}*>(instance)->{{class_field_name}} = *static_cast<{{{class_field_type}}}*>(field_value);}
        static void* get_{{class_field_name}}(void* instance){ return static_cast<void*>(&(static_cast<{{class_name}}*>(instance)->{{class_field_name}}));}
        static bool isArray_{{class_field_name}}(){ {{#class_field_is_vector}}return true;{{/class_field_is_vector}}{{^class_field_is_vector}}return false;{{/class_field_is_vector}} }
        {{/class_field_defines}}
    };
}//namespace TypeFieldReflectionOparator
```

这部分中需要填空的地方对应 parser/generator/generator.cpp

```
void GeneratorInterface::genClassRenderData(std::shared_ptr<Class> class_temp, Mustache::data& class_def)
{
    class_def.set("class_name", class_temp->getClassName());
    class_def.set("class_base_class_size", std::to_string(class_temp->m_base_classes.size()));
    class_def.set("class_need_register", true);

    if (class_temp->m_base_classes.size() > 0)
    {
        Mustache::data class_base_class_defines(Mustache::data::type::list);
        class_def.set("class_has_base", true);
        for (int index = 0; index < class_temp->m_base_classes.size(); ++index)
        {
            Mustache::data class_base_class_def;
            class_base_class_def.set("class_base_class_name", class_temp->m_base_classes[index]->name);
            class_base_class_def.set("class_base_class_index", std::to_string(index));
            class_base_class_defines.push_back(class_base_class_def);
        }
        class_def.set("class_base_class_defines", class_base_class_defines);
    }

    Mustache::data class_field_defines = Mustache::data::type::list;
    genClassFieldRenderData(class_temp, class_field_defines);
    class_def.set("class_field_defines", class_field_defines);
}
```

详细的先不看了，这里大概理解一下：这部分代码完成了完型填空

## 构建并使用 PiccoloParser

PiccoloParser 并不是直接就存在的，而是编译生成的，所以大家就知道我们现在又要去捣鼓 cmake 了

Parser 会分析代码，所以必然是通过 cmake 提前被调用

RTFSC 后，我们发现了 CMake 中调用 Parser 的地方——`precompile.cmake`

具体来说，就是这一段

```
# Called first time when building target
add_custom_target(${PRECOMPILE_TARGET} ALL

# COMMAND # (DEBUG: DON'T USE )
#     this will make configure_file() is called on each compile
#   ${CMAKE_COMMAND} -E touch ${PRECOMPILE_PARAM_IN_PATH}a

# If more than one COMMAND is specified they will be executed in order...
COMMAND
  ${CMAKE_COMMAND} -E echo "************************************************************* "
COMMAND
  ${CMAKE_COMMAND} -E echo "**** [Precompile] BEGIN "
COMMAND
  ${CMAKE_COMMAND} -E echo "************************************************************* "

COMMAND
    ${PRECOMPILE_PARSER} "${PICCOLO_PRECOMPILE_PARAMS_PATH}"  "${PARSER_INPUT}"  "${ENGINE_ROOT_DIR}/source" ${sys_include} "Piccolo" 0
### BUILDING ====================================================================================
COMMAND
    ${CMAKE_COMMAND} -E echo "+++ Precompile finished +++"
)

```

第四个 COMMAND 调用了`PRECOMPILE_PARSER`，接下来我们深入看看这部分的输入输出

可以看到`set(PRECOMPILE_PARSER ${PRECOMPILE_TOOLS_PATH}/PiccoloParser.exe)`设置了 parser 变量，其中`PRECOMPILE_TOOLS_PATH`为`set(PRECOMPILE_TOOLS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin")`

而`PiccoloParser`在 meta\_parser/CMakeLists.txt 中被构建

`PARSER_INPUT`为`set (PARSER_INPUT ${CMAKE_BINARY_DIR}/parser_header.h)`，根据[这篇文章](https://zhuanlan.zhihu.com/p/493894595)的解释，是_根据之前的 PILOT\_PRECOMPILE\_PARAMS\_PATHjson 文件中所有需要引用到的文件的合并#include_

后面的`${ENGINE_ROOT_DIR}/source`应该是文件生成的目标位置

`${sys_include}`应该是一些系统库需要被包含进来的部分

最后的部分是一些参数其他参数，暂时不管

大头是这个`${PICCOLO_PRECOMPILE_PARAMS_PATH}`，我们可以看到有两处关系到它

```
set(PICCOLO_PRECOMPILE_PARAMS_PATH "${PRECOMPILE_TOOLS_PATH}/precompile.json")
configure_file(${PICCOLO_PRECOMPILE_PARAMS_IN_PATH} ${PICCOLO_PRECOMPILE_PARAMS_PATH})
```

重点是第二句`configure_file`，关于这个命令可以看看[这篇文章](https://link.zhihu.com/?target=http%3A//t.csdn.cn/CCAzZ)，大概就是把参数中前者进行一些宏替换生成后者

前者的文本为`@PICCOLO_RUNTIME_HEADS@,@PICCOLO_EDITOR_HEADS@`，通过`@`把中间的宏替换成了 CMake 设置的变量，在 Piccolo 源码中就是 runtime/CMakeLists.txt 的

```
### precompile
# set global vari used by precompile
set(PICCOLO_RUNTIME_HEADS "${HEADER_FILES}" PARENT_SCOPE)# PARENT_SCOPE: 父作用域
# 我把HEAD_FILES改名为RUNTIME_HEADERS了所以这里也应该替换
```

另一个变量也大同小异

现在我们知道了，这个`configure_file`指令会生成一个`precompile.json`，内容为 source 中的一些 header 的路径，而这些 Header 就是需要被检查的东西

最后通过`set(PRECOMPILE_TARGET "PiccoloPreCompile")`指令设定预编译目标的符号

此时回到 engine 底下的 CMakeLists，加入如下行调用 PiccoloPreCompile.cmake，`include(source/precompile/precompile.cmake)`

这样我们就在 engine/bin 中生成了 precompile.json，这个 json 文件会指定 PiccoloParser 需要分析哪些文件

不过这个还不够，我们的目标是要构建使用 PiccoloParser，现在我们还差几行

```
add_subdirectory(source/meta_parser)
set(CODEGEN_TARGET "PiccoloPreCompile")
include(source/precompile/precompile.cmake)
set_target_properties("${CODEGEN_TARGET}" PROPERTIES FOLDER "Engine" )
add_dependencies(PiccoloRuntime "${CODEGEN_TARGET}")
add_dependencies("${CODEGEN_TARGET}" "PiccoloParser")
```

通过这几行我们就构建了 PiccoloParser，并且要求 PiccoloRuntime 依赖与 PiccoloParser

现在编译我们的代码，你会发现你多了这些东西

![](https://pic1.zhimg.com/v2-9142e693c2a1b411880bb1f39aae6ad4_b.jpg)

Image

![](https://pic4.zhimg.com/v2-3c2042a681a4211795a2efac03628f83_b.jpg)

Image

### 还有些 bug

看来我们成功生成了代码!!!????

还是少了点东西，没错，我现在已经往 vector2 中加入了`REFLECTION_TYPE`之类的宏，但是没有`vector2.reflection.gen.h`出现，这是为什么呢？

RTFSC 后，我们发现在`reflection.h`中有这个东西`__REFLECTION_PARSER__`，同时在`parser.h`中有这样的东西

```
std::vector<const char*> arguments = {{"-x",
                                       "c++",
                                       "-std=c++11",
                                       "-D__REFLECTION_PARSER__",
                                       "-DNDEBUG",
                                       "-D__clang__",
                                       "-w",
                                       "-MG",
                                       "-M",
                                       "-ferror-limit=0",
                                       "-o clangLog.txt"}};
```

第四行就是启用这个宏

现在我们就明白了，parser 是先启用这个宏然后分析这些东西来生成对应的代码

```
#if defined(__REFLECTION_PARSER__)
#define META(...) __attribute__((annotate(#__VA_ARGS__)))
#define CLASS(class_name, ...) class __attribute__((annotate(#__VA_ARGS__))) class_name
#define STRUCT(struct_name, ...) struct __attribute__((annotate(#__VA_ARGS__))) struct_name

REFLECTION_TYPE(Vector2)
CLASS(Vector2, Fields)
    REFLECTION_BODY(Vector2);
```

所以在`reflection.h`中加入对应宏定义即可

可以看到现在已经生成了我们想要的文件了

![](https://pic2.zhimg.com/v2-d067533fd84bb53eeff711af91d24095_b.jpg)

Image

## 想要试用一下反射

现在我们尝试用一下反射，不过马上就遇到了问题，

![](https://pic4.zhimg.com/v2-afa382a4e865d5709cfd96c72d0f457f_b.jpg)

Image

看来不得不先去解决一下 json 与序列化的问题了，不过现在篇幅已经比较长了，关于序列化的问题，就在下一篇文章中解决吧

___

这一节我们成功实现了反射代码的自动生成，下一节将会导入 json 库并实现序列化

> 本文使用 [Zhihu On VSCode](https://zhuanlan.zhihu.com/p/106057556) 创作并发布

本系列是笔者学习王希老师的 GAMES104 现代游戏引擎入门课程时抄写 Piccolo 源码的笔记，系列最终目标是王希老师所言“完成 104 后，人手一个小引擎”“最后能做个多人网络对战游戏”

笔者代码在[此处](https://link.zhihu.com/?target=https%3A//github.com/AmamiyaRenn/MyPiccolo/tree/v2.5.3)

我们已经完成反射代码自动生成的部分，这一节，我们将会完成最后一步——用 json11 库实现序列化，从而让我们能把程序数据与磁盘中的 json 数据进行交互

## 序列化

之所以需要序列化，是我们希望能把类的数据简单的保存到硬盘中，同时又能简单的从硬盘中读取类数据——字符串就是一种很好的方式，例如上面的 json 文件；事实上，这个文件可以通过反序列化的方式转化为类对象数据，在通过反射机制赋值到对应的对象上

关于序列化，可以参考一下[这篇文章](https://zhuanlan.zhihu.com/p/75209197)

-   **序列化(serialization)**：把类对象转化为字节序列
-   **反序列化(deserialization)**：把字节序列恢复为类对象

Piccolo 使用 json11 作为 json 序列化工具，现在我们要把这个库导入

导入的方法也简单，我直接复制了源码中 3rdparty 的 json11 文件夹，然后在 engine/source/runtime 的 CMakeLists 中加入如下代码，把库链接进来

```
set(json_include ${THIRD_PARTY_DIR}/json11)
add_library(json11 ${json_include}/json11.cpp)
target_link_libraries(${TARGET_NAME} PRIVATE $<BUILD_INTERFACE:json11>)
```

关于里面的`PRIVATE`关键字，可以看看[这篇文章](https://link.zhihu.com/?target=http%3A//t.csdn.cn/XPQbD)与[这篇文章](https://link.zhihu.com/?target=http%3A//t.csdn.cn/hXngw)，再类比一下 C++中的关键字就能大概理解；所以这里用 private 会让 json 库被 link 到 PiccoloRuntime 中，但如果外界要用 PiccoloRuntime，它会和一个声明了私有变量的类一样，没办法找到 json 的符号（符号未导出）

当然，不要忘记把头文件包含进来

```
# json
target_include_directories(
    ${TARGET_NAME}
    PUBLIC $<BUILD_INTERFACE:${json_include}>
)
```

## 玩一玩 Json11 库

导入 json 库后可以先学习一下这个库怎么用，比如阅读一下这个库的[GitHub](https://link.zhihu.com/?target=https%3A//github.com/dropbox/json11)

下图给出了一些使用样例

![](https://pic1.zhimg.com/v2-504a444a79f1061eedc93b1d1b398b64_b.jpg)

Image

## 完成 Serializer 类

下一步，我们要使用 Json 库进行序列化，所以接下来我们主要工作在`serializer`文件夹

我们在第一篇文章中曾经卡在了`Serializer::read`处，所以现在我们先从这个函数入手

```
static void* constructorWithJson(const Json& json_context)
{
    Vector2* ret_instance = new Vector2;
    Serializer::read(json_context, *ret_instance);
    return ret_instance;
}
```

这个`read`函数对应的是`all_serializers.ipp`的这个函数

```
template<>
Vector2& Serializer::read(const Json& json_context, Vector2& instance){
    assert(json_context.is_object());
    if(!json_context["x"].is_null())
        Serializer::read(json_context["x"], instance.x);
    if(!json_context["y"].is_null())
        Serializer::read(json_context["y"], instance.y);
    return instance;
}
```

根据我们把玩 json11 库得到的经验，可以知道`json_context["x"]`是一个键值对应，对应着一个仅包括数字的`Json`类对象，所以中间的这个`read`函数又是对应了`serializer.cpp`的这个函数

```
template<>
float& Serializer::read(const Json& json_context, float& instance)
{
    assert(json_context.is_number());
    return instance = static_cast<float>(json_context.number_value());
}
```

读代码可知，此处`instance.x`被赋值为了`json_context["x"]`对应的数字，也就完成了构造 Vector2 实例在反序列化读入 json 作为初始值的目的

那么我们回到`vector2.reflection.gen.h`，看看还有什么不懂的

```
static Json writeByName(void* instance) { return Serializer::write(*(Vector2*)instance); }
Json Serializer::write(const Vector2& instance){
    Json::object  ret_context;
    ret_context.insert_or_assign("x", Serializer::write(instance.x));
    ret_context.insert_or_assign("y", Serializer::write(instance.y));
    return  Json(ret_context);
}
template<>
Json Serializer::write(const float& instance){ return Json(instance);}
```

现在看来，这个`writeByName`函数我们已经能懂了——就是外界传入一个 Vector2 实例，然后序列化为 json 类后返回

那么 Serializer 类我们还有什么不懂的部分呢，现在我们打开`serializer.h`，发现里面是直接与之前反射相关

```
template<typename T>
static T*& read(const Json& json_context, Reflection::ReflectionPtr<T>& instance)
{
    std::string type_name = json_context["$typeName"].string_value();
    instance.setTypeName(type_name);
    return readPointer(json_context, instance.getPtrReference());
}

template<typename T>
static T*& readPointer(const Json& json_context, T*& instance)
{
    assert(instance == nullptr);
    std::string type_name = json_context["$typeName"].string_value();
    assert(!type_name.empty());
    if ('*' == type_name[0])
    {
        instance = new T;
        read(json_context["$context"], *instance);
    }
    else
    {
        instance = static_cast<T*>(
            Reflection::TypeMeta::newFromNameAndJson(type_name, json_context["$context"]).m_instance);
    }
    return instance;
}

ReflectionInstance TypeMeta::newFromNameAndJson(std::string type_name, const Json& json_context)
{
    auto iter = m_class_map.find(type_name);
    if (iter != m_class_map.end())
    {
        return ReflectionInstance(TypeMeta(type_name), (std::get<1>(*iter->second)(json_context)));
    }
    return ReflectionInstance();
}
```

当然，我们不是很慌，慢慢看下来总是能看明白的；这里我们直接找到实例来看看

```
"objects": [
    {
      "name": "Player",
      "instanced_components": [
        {
          "$typeName": "TransformComponent",
          "$context": {
            "transform": {
              "position": {
                "x": 3,
                "y": 3,
                "z": 0
              },
              "rotation": {
                "w": 0.70710678118654752,
                "x": 0,
                "y": 0,
                "z": 0.70710678118654752
              },
              "scale": {
                "x": 1,
                "y": 1,
                "z": 1
              }
            }
          }
        }
      ],
      "definition": "asset/objects/character/player/player.object.json"
    },
]
```

努力阅读代码和 json 文件，我们大概猜出了这段代码做了什么——根据`${typeName}`找到被反射的类`TransformComponent`，然后反序列化`$context`的数据作为初值

```
REFLECTION_TYPE(TransformComponent)
CLASS(TransformComponent : public Component, WhiteListFields)
{
    REFLECTION_BODY(TransformComponent)
protected:
    META(Enable)
    Transform m_transform;

    Transform m_transform_buffer[2];
    size_t    m_current_index {0};
    size_t    m_next_index {1};
}
```

那么`write`相关的函数我们也就能简单的搞明白了

现在搞明白这些后，就可以开抄代码了！

### 遇到 bug 了

在我高高兴兴抄完代码后，编译~~~通不过......

![](https://pic1.zhimg.com/v2-d28b98680977af7f7213ffef4935c440_b.jpg)

Image

这里后面应该是`= *static_cast<std::vector<int*>*>(field_value);`才对，然后我往上一看

![](https://pic3.zhimg.com/v2-89456a15f730e0a95c8c95af7e57d952_b.jpg)

Image

果不其然，这里也错了，应该是`return "std::vector<int*>;`才对；种种迹象表面，我的代码自动生成工具出问题了！

出问题的地方对应着 mustache 的`class_field_type`字段，我首先怀疑是我之前构建出了问题，不过马上否定了，因为其他部分都没问题，不可能就这个部分出问题

然后我怀疑了一下，怕不是我忘记在`meta_example.h`里加入`#include <vector>`了

我看了看我的文件，果不其然，还真没加；于是我赶紧加上，重新编译——这次总算好了

究其根源我觉得是因为分析工具从底层来说也基于编译器，如果没`#include`根本找都找不到符号肯定也就无法分析了

上面说“总算好了”，其实也没好，我遇到了一堆链接问题——写了成员函数原型但没写实现；在我补完实现后，总算编译运行成功了，又看到了熟悉的三角形

## 试玩一下带序列化功能的反射吧！

我们现在终于可以把`meta_example.cpp`补完了，补好后，在 main 函数`editor->initialize()`前加入`Piccolo::metaExample();`，然后编译运行程序，得到了如下输出

![](https://pic1.zhimg.com/v2-9385a5ba82468b620e0162b189547a50_b.jpg)

Image

这证明了我们至今为止的所有事情都做对了！！！

___

经过了漫长的三天，终于完成了反射与序列化部分的抄写，接下来会回到正题——引入简单的资产系统与文件系统，帮助我们进入 3D 图形时代！

> 本文使用 [Zhihu On VSCode](https://zhuanlan.zhihu.com/p/106057556) 创作并发布