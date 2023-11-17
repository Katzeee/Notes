#typescript
## 2023.11.17

## for-loop

- C-style

```ts
for (let i = 0; i < 3; i++) {
  console.log ("Block statement execution no." + i);
}
```

- for in

> https://stackoverflow.com/questions/1886179/why-does-javascript-turn-array-indexes-into-strings-when-iterating

```ts
let arr = [10, 20, 30, 40];

for (var index1 in arr) {
  console.log(index1); // prints indexes: 0, 1, 2, 3
}
```

!!! Warning typeof index1 is string

- for of

```ts
let arr = [10, 20, 30, 40];

for (var val of arr) {
  console.log(val); // prints values: 10, 20, 30, 40
}
```

- forEach

```ts
let arr = [10, 20, 30, 40];

arr.forEach((value, index) => {
  console.log(value, index); 
})
```