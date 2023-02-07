const { VueLoaderPlugin } = require('vue-loader');
const HtmlWebpackPlugin = require('html-webpack-plugin');
const HtmlWebpackExternalsPlugin = require('html-webpack-externals-plugin');
const path = require('path');

const flashDir = path.resolve(__dirname, '../data');

module.exports = {
  stats: {
    errorDetails: true,
  },
  externals: {
    vue: 'Vue',
  },
  devServer: {
    static: flashDir,
  },
  module: {
    rules: [
      {
        test: /\.vue$/,
        loader: 'vue-loader'
      },
      {
        test: /\.js$/,
        loader: 'babel-loader'
      },
      {
        test: /\.(sa|sc|c)ss$/,
        use: ['style-loader', 'css-loader', 'sass-loader'],
      },
    ],
  },
  plugins: [
    new VueLoaderPlugin(),
    new HtmlWebpackPlugin({
      appMountId: 'app',
      template: 'index.html',
      title: 'ok2wake',
      hash: true,
    }),
    new HtmlWebpackExternalsPlugin({
      externals: [
        {
          module: 'pure-base',
          entry: {
            path: 'https://unpkg.com/purecss@2.1.0/build/base-min.css',
            type: 'css'
          }
        },
        {
          module: 'pure',
          entry: {
            path: 'https://unpkg.com/purecss@2.1.0/build/pure-min.css',
            type: 'css'
          }
        },
        {
          module: 'pure-grid',
          entry: {
            path: 'https://unpkg.com/purecss@2.1.0/build/grids-responsive-min.css',
            type: 'css'
          }
        },
        {
          module: 'pure-forms',
          entry: {
            path: 'https://unpkg.com/browse/purecss@2.1.0/build/forms-min.css',
            type: 'css'
          }
        },
        {
          module: 'fontawesome',
          entry: {
            path: 'https://kit.fontawesome.com/5805201f22.js',
            type: 'js'
          }
        },
        {
          module: 'vue',
          entry: {
            path: 'https://unpkg.com/vue@3',
            type: 'js'
          }
        }
      ]
    }),
  ],
  output: {
    path: flashDir,
    clean: {
      keep: /\.json$/
    },
    publicPath: '/',
  }
}