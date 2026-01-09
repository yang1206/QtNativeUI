import { defineConfig } from 'vitepress'
import llmstxt from 'vitepress-plugin-llms'
export default defineConfig({
  title: "QtNativeUI",
  description: "现代化的 Qt UI 库",
  lastUpdated: true,
  cleanUrls: true,
  metaChunk: true,
  head: [
    // 基本meta信息
    ['meta', { name: 'viewport', content: 'width=device-width, initial-scale=1.0' }],
    ['meta', { name: 'description', content: '现代化的 Qt UI 组件库，基于 Microsoft Fluent Design 设计体系，提供跨平台的原生外观控件' }],
    ['meta', { name: 'keywords', content: 'Qt, UI库, Fluent Design, C++, 跨平台, 组件库, Windows, macOS, Linux, Mica, Acrylic' }],
    ['meta', { name: 'author', content: 'yang1206' }],
    
    // Open Graph / Facebook
    ['meta', { property: 'og:type', content: 'website' }],
    ['meta', { property: 'og:title', content: 'QtNativeUI - 现代化的 Qt UI 组件库' }],
    ['meta', { property: 'og:description', content: '基于 Microsoft Fluent Design 设计体系的现代化 Qt UI 组件库，提供跨平台的原生外观控件，支持 Windows Mica/Acrylic 效果' }],
    ['meta', { property: 'og:url', content: 'https://github.com/yang1206/QtNativeUI' }],
    ['meta', { property: 'og:site_name', content: 'QtNativeUI' }],
    ['meta', { property: 'og:locale', content: 'zh_CN' }],
    
    // Twitter Card
    ['meta', { name: 'twitter:card', content: 'summary_large_image' }],
    ['meta', { name: 'twitter:title', content: 'QtNativeUI - 现代化的 Qt UI 组件库' }],
    ['meta', { name: 'twitter:description', content: '基于 Microsoft Fluent Design 设计体系的现代化 Qt UI 组件库' }],
    
    // 技术相关
    ['meta', { name: 'theme-color', content: '#0078d4' }],
    ['meta', { name: 'msapplication-TileColor', content: '#0078d4' }],
    ['meta', { name: 'referrer', content: 'no-referrer-when-downgrade' }],
    
    // 图标和manifest
    ['link', { rel: 'icon', type: 'image/svg+xml', href: '/logo.svg' }],
    ['link', { rel: 'icon', type: 'image/png', sizes: '32x32', href: '/favicon-32x32.png' }],
    ['link', { rel: 'icon', type: 'image/png', sizes: '16x16', href: '/favicon-16x16.png' }],
    ['link', { rel: 'apple-touch-icon', sizes: '180x180', href: '/apple-touch-icon.png' }],
    
    // 预连接优化
    ['link', { rel: 'preconnect', href: 'https://fonts.googleapis.com' }],
    ['link', { rel: 'preconnect', href: 'https://fonts.gstatic.com', crossorigin: '' }],
  ],
  locales: {
    root: {
      label: '简体中文',
      lang: 'zh-CN',
      dir: 'ltr',
    },
  },
  themeConfig: {
    search: {
      provider: 'local',
    },
  },
  vite: {
    plugins: [
      llmstxt(
      )
    ],
  }
})
