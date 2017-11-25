const {
  setTickCallback,
  loop,
  kRejected,
  isPromise,
} = require('bindings')('syncify');

setTickCallback(process._tickCallback);

module.exports = (promise) => {
  if (!isPromise(promise))
    return promise;

  const [state, result] = loop(promise);

  if (state === kRejected)
    throw result;

  return result;
};
