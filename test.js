const deasync = require('.');

const p0 = new Promise((r) => {
  setTimeout(() => r('works'), 1000);
});

console.log(deasync(p0));

const p1 = Promise.reject(new Error('aaaa'));

deasync(p1);
