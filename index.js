const {
  loop,
  kRejected,
  isPromise,
} = require('bindings')('syncify');

module.exports = (promise) => {
  if (!isPromise(promise))
    return promise;

  const [state, result] = loop(promise, process._tickCallback);

  if (state === kRejected)
    throw result;

  return result;
};
