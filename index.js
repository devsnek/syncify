'use strict';

const {
  loop,
  kRejected,
  setTickCallback,
  isPromise,
} = require('bindings')('syncify');

setTickCallback(process._tickCallback);

const noop = () => {}; // eslint-disable-line no-empty-function

module.exports = (promise) => {
  if (!isPromise(promise))
    return promise;

  promise.catch(noop);

  const [state, result] = loop(promise);

  if (state === kRejected)
    throw result;

  return result;
};
