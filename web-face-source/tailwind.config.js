module.exports = {
  darkMode: 'media', // or 'media' or 'class'
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
    extend: {
      opacity: [
        'disabled'
      ],
      backgroundColor: [
        'disabled'
      ],
      cursor: [
        'disabled'
      ]
    }
  },
  plugins: [
    require('flowbite/plugin')
  ]
}
