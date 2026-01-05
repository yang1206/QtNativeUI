import { defineConfig } from 'vitepress'
import llmstxt from 'vitepress-plugin-llms'
export default defineConfig({
  title: "QtNativeUI",
  description: "现代化的 Qt UI 库",
  lastUpdated: true,
  cleanUrls: true,
  metaChunk: true,
  locales: {
    root: {
      label: '简体中文',
      lang: 'zh-CN',
      dir: 'ltr',
    },
  },
  vite: {
    plugins: [
      llmstxt(
      )
    ],
  }
})
