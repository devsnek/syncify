const tap = require('tap');
const syncify = require('.');
const util = require('util');
const setTimeoutAsync = util.promisify(setTimeout);

tap.plan(3);

tap.type(syncify, 'function', 'is a function');

const start = Date.now();
syncify(setTimeoutAsync(1000));
tap.ok(Date.now() - start >= 1000, 'waits for the promise to resolve');

tap.throws(() => syncify(Promise.reject(new Error('this is an error'))),
  'this is an error', 'throws when the promise rejects');

tap.end();

