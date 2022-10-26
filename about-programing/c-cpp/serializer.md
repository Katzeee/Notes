2022.10.26
---

## Example

```cpp
class RoboCat : public GameObject {
public:
  RoboCat() : mHealth(10), mMeowCount(3) {}
  virtual void Update();
private:
  int32_t mHealth;
  int32_t mMeowCount;
  GameObjcet* mHomeBase;
  char mName[128];
  std::vector<int32_t> mMiceIndices;
};
```

Serialize this object may face following problems.

- The first four bytes of RoboCat object is a virtual table pointer. So you can't simply serialize this object and then pass it to another system. So does the mHomeBase pointer.

- `mName` may only store few bytes like `"Fuzzy\0"`, but we will serialize the whole 128 bytes.

- Is copying `std::vector` safe?

## Stream

Class `OutputMemoryStream` allows you to customize the methods to write the object information to the stream.

```cpp
template<typename T>
void OutputMemoryStream::Write(T inData) {
  static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value);
  Write(&inData, sizeof(Data));
}
```

Thus, we can serialize the object by this way:

```cpp
void RoBoCat::Write(OutputMemoryStream& inStream) const {
  inStream.Write(mHealth);
  inStream.Write(mMeowCount);
  inStream.Write(mName, 128);
  // no solution for std::vector and pointer attribute
}
```

## Endianness

One method to realize byte swap function.

```cpp
inline uint16_t ByteSwap2(uint16_t inData) {
  return (inData >> 8) | (inData << 8);
}

inline uint32_t ByteSwap4(uint32_t inData) {
  ...
}

...

template <typename T, size_t tSize> 
T ByteSwap(T inData) const {
  assert(0);
}

template <typename T>
T ByteSwap<T, 2>(T inData) {
  auto temp = reinterpret_cast<uint16_t>(inData);
  return reinterpret_cast<T>(ByteSwap2(temp));
}

template <typename T>
T ByteSwap<T, 4>(T inData) {
  ...
}

...

template <typename T>
T Swap(T inData) {
  return ByteSwap<T, sizeof(T)>(inData);
}
```

Now, write can support different endianness conversion:

```cpp
template<typename T>
void OutputMemoryStream::Write(T inData) {
  static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value);
  if (STREAM_ENDIANNES == PLATFORM_ENDIANNESS) {
    Write(&inData, sizeof(Data));
  } else {
    Write(&Swap(inData), sizeof(inData));
  }
}
```

## BitStream

BitStream allows you to serialize data with bits as the smallest unit not bytes.

```cpp
void OutputMemoryBitStream::WriteBits(const void* inData, size_t inBitCount) {
  ...
}
```

## Non-POD

### Embedded

For `std::vector`:

```cpp
template <typename T>
void Write(const std::vector<T>& inVector) {
  Write(inVector.size());
  for (const T& it : inVector) {
    Write(it);
  }
}
```
### Linking

`std::vector<RoBoCat*>` can't use above method. So consider use a unique id to identify each object.

```cpp
void Write(const GameObject* inGameObject) {
  Write(mLinkingContext->GetNetworkId(inGameObject));
}
```
