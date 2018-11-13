module.exports = {
  outputDir: '../../data/cp/',
  baseUrl: process.env.NODE_ENV === 'production' ? '/cp/' : '/',
  filenameHashing: false,
  indexPath: 'index.htm',
  productionSourceMap: false,
}
