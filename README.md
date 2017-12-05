# syncify

(this is not something you should use in prod)

```javascript
const syncify = require('@snek/syncify');

const p0 = new Promise((r) => {
  setTimeout(() => r('works'), 1000);
});

// blocks for 1000ms
console.log(syncify(p0)); // 'works!'

const p1 = Promise.reject(new Error('aaaa'));

syncify(p1); // Error: 'aaaa'
```
