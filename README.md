# deasync

```js
const deasync = require('deasync');

const p0 = new Promise((r) => {
  setTimeout(() => r('works'), 1000);
});

// logs `'works'`
console.log(deasync(p0));

const p1 = Promise.reject(new Error('aaaa'));

// throws
deasync(p1);
```
