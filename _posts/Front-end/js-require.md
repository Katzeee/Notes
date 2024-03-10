#nodejs
## 2023.08.16

## require

`require` is used to load a module and returns the export object of it.


```javascript
// module.js
module.exports = {
  sayHello: function() {
    return 'Hello, world!';
  }
};
```

```javascript
// app.js
var myModule = require('./module');

console.log(myModule.sayHello()); // 输出 "Hello, world!"
```

`require('./module')` returns the `module.exports` object in `module.js`