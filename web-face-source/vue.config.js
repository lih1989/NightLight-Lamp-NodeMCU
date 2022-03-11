// eslint-disable-next-line @typescript-eslint/no-var-requires
const path = require('path')

module.exports = {
  productionSourceMap: false,
  outputDir: path.resolve(__dirname, '../firmware/data'),
  publicPath: '',
  filenameHashing: false,
  devServer: {
    // host: 'crm-local.anflat.ru',
    port: 7777
    // key: fs.readFileSync('ssl/privkey.key'),
    // cert: fs.readFileSync('ssl/fullchain.crt'),
    // https: true,
    // hotOnly: false
  },
  // transpileDependencies: [
  //   'vuetify'
  // ],
  // css: {
  //   loaderOptions: {
  //     sass: {
  //       additionalData: '@import "~@/assets/scss/_vuetify-customization-variables.scss"'
  //     }
  //   }
  // },
  // Разбивать js на куски при сборке
  // configureWebpack: {
  //   optimization: {
  //     splitChunks: {
  //       minSize: 10000,
  //       maxSize: 100000
  //     }
  //   }
  // },
  pwa: {
    name: 'RRC',
    themeColor: '#e4300e',
    workboxOptions: {
      skipWaiting: true,
      clientsClaim: true
    }
  }
}
