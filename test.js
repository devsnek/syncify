const syncify = require('.');

const p0 = new Promise((r) => {
  setTimeout(() => r('works'), 1000);
}).then((r) => r);

console.log(syncify(p0));

const p1 = Promise.reject(new Error('aaaa'));

syncify(p1);
