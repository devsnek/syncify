# syncify

(this is not something you should use in prod)

```js
const syncify = require('@snek/syncify');

const p0 = new Promise((r) => {
  setTimeout(() => r('works'), 1000);
});

// logs `'works'`
console.log(syncify(p0));

const p1 = Promise.reject(new Error('aaaa'));

// throws
syncify(p1);
```
