module.exports = {
  darkMode: false, // or 'media' or 'class'
  purge: ['./index.html', './src/**/*.{vue,js,ts,jsx,tsx}'],
  content: [
    './index.html',
    './src/**/*.{vue,js,ts,jsx,tsx}',
    './node_modules/flowbite/**/*.js'
  ],
  theme: {
    extend: {}
  },
  variants: {
    extend: {}
  },
  plugins: [
    require('flowbite/plugin')
  ]
}
