#nodejs
## 2023.10.12

## `Promise`

Promise is an object in javascript, which has 3 status: **pending**, **fulfilled**, **rejected**.

Simple example:

```javascript
let promise = new Promise(function(resolve, reject){
    let res = true; // hypothetical result of an asynchronous operation
    if (res)
        resolve('Success!');
    else
        reject('Failure!');
});

promise.then((message) => {
    console.log(message);
}).catch((message) => {
    console.log(message);
})
```

## `resolve` and `reject`

`resolve` and `reject` are two functions provided by `Promise` constructor which are used to change the status of `Promise`.

- `resolve` should be called when asynchronous operation succeed, passing the operation's result as its first arguement.
- `reject` should be called when asynchronous operation fail, passing the error code or reason as its first arguement.

## `then` and `catch`

`then` and `catch` are asynchronous, as they will be stroed in a task queue and be processed when the promise finished.

## `await` and `async`

`await` a promise waits until the promise is finished.

!!! Note `await` can only be used inside a `async` function, which means you should add `async` declaration for your function

## Difference of `await` promise and sequential sync execute

`await` only block the current code execution, while other tasks are still executing like ui interaction, event listener etc. 


