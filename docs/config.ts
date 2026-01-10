import {defineAdditionalConfig, type DefaultTheme} from 'vitepress'

export default defineAdditionalConfig({
    themeConfig: {
        logo: '/logo.svg',
        search: {options: searchOptions(), provider: 'local'},
        nav: [
            {text: '首页', link: '/index'},
            {text: '指南', link: '/guide/getting-started'},
            {text: '组件', link: '/components/NLabel'},
            {text: '工具', link: '/tools/NTheme'},
            {text: 'GitHub', link: 'https://github.com/yang1206/QtNativeUI'}
        ],
        sidebar: {
            '/guide/': [
                {
                    text: '开始使用',
                    items: [
                        {text: '快速开始', link: '/guide/getting-started'},
                        {text: '设计理念', link: '/guide/design'},
                        {text: '部署指南', link: '/guide/deployment'},
                        {text: '贡献指南', link: '/guide/contributing'},
                        {text: '致谢', link: '/guide/acknowledgments'}
                    ]
                }
            ],
            '/components/': [
                {
                    text: '基础控件',
                    items: [
                        {text: 'NLabel', link: '/components/NLabel'},
                        {text: 'NPushButton', link: '/components/NPushButton'},
                        {text: 'NToolButton', link: '/components/NToolButton'},
                        {text: 'NHyperlinkButton', link: '/components/NHyperlinkButton'},
                        {text: 'NToggleButton', link: '/components/NToggleButton'},
                        {text: 'NDropDownButton', link: '/components/NDropDownButton'}
                    ]
                },
                {
                    text: '输入控件',
                    items: [
                        {text: 'NLineEdit', link: '/components/NLineEdit'},
                        {text: 'NTextEdit', link: '/components/NTextEdit'},
                        {text: 'NPlainTextEdit', link: '/components/NPlainTextEdit'},
                        {text: 'NSpinBox', link: '/components/NSpinBox'},
                        {text: 'NDoubleSpinBox', link: '/components/NDoubleSpinBox'},
                        {text: 'NSlider', link: '/components/NSlider'},
                        {text: 'NRangeSlider', link: '/components/NRangeSlider'},
                        {text: 'NComboBox', link: '/components/NComboBox'},
                        {text: 'NAutoSuggestBox', link: '/components/NAutoSuggestBox'}
                    ]
                },
                {
                    text: '选择控件',
                    items: [
                        {text: 'NCheckBox', link: '/components/NCheckBox'},
                        {text: 'NRadioButton', link: '/components/NRadioButton'},
                        {text: 'NToggleSwitch', link: '/components/NToggleSwitch'}
                    ]
                },
                {
                    text: '导航',
                    items: [
                        {text: 'NNavigationView', link: '/components/NNavigationView'},
                        {text: 'NTabWidget', link: '/components/NTabWidget'},
                        {text: 'NTabBar', link: '/components/NTabBar'},
                        {text: 'NPivot', link: '/components/NPivot'},
                        {text: 'NMenu', link: '/components/NMenu'}
                    ]
                },
                {
                    text: '容器',
                    items: [
                        {text: 'NMainWindow', link: '/components/NMainWindow'},
                        {text: 'NGroupBox', link: '/components/NGroupBox'},
                        {text: 'NStackedWidget', link: '/components/NStackedWidget'},
                        {text: 'NScrollArea', link: '/components/NScrollArea'}
                    ]
                },
                {
                    text: '显示控件',
                    items: [
                        {text: 'NProgressBar', link: '/components/NProgressBar'},
                        {text: 'NProgressRing', link: '/components/NProgressRing'},
                        {text: 'NScrollBar', link: '/components/NScrollBar'},
                        {text: 'NToolTip', link: '/components/NToolTip'}
                    ]
                },
                {
                    text: '项目视图',
                    items: [
                        {text: 'NListView', link: '/components/NListView'},
                        {text: 'NTreeView', link: '/components/NTreeView'}
                    ]
                },
                {
                    text: '对话框和弹出窗口',
                    items: [
                        {text: 'NDialog', link: '/components/NDialog'},
                        {text: 'NContentDialog', link: '/components/NContentDialog'},
                        {text: 'NFlyout', link: '/components/NFlyout'},
                        {text: 'NCalendarWidget', link: '/components/NCalendarWidget'},
                        {text: 'NCalendarDatePicker', link: '/components/NCalendarDatePicker'},
                        {text: 'NInfoBar', link: '/components/NInfoBar'}
                    ]
                }
            ],
            '/tools/': [
                {
                    text: '工具和系统',
                    items: [
                        {text: 'NTheme', link: '/tools/NTheme'},
                        {text: 'NConfig', link: '/tools/NConfig'},
                        {text: 'NIcon', link: '/tools/NIcon'},
                        {text: 'NColors', link: '/tools/NColors'},
                        {text: 'NFluentColors', link: '/tools/NFluentColors'},
                        {text: 'NEnums', link: '/tools/NEnums'}
                    ]
                }
            ]
        },
        socialLinks: [
            {icon: 'github', link: 'https://github.com/yang1206/QtNativeUI'}
        ],
        editLink: {
            pattern: 'https://github.com/yang1206/QtNativeUI/edit/main/docs/:path',
            text: '在 GitHub 上编辑此页面'
        },
        footer: {
            message: '基于 LGPL-3.0 许可发布',
            copyright: 'Copyright © 2025-present yang1206 Contributors'
        },
        docFooter: {
            prev: '上一页',
            next: '下一页'
        },
        outline: {
            label: '页面导航'
        },
        lastUpdated: {
            text: '最后更新于'
        },
        notFound: {
            title: '页面未找到',
            quote:
                '但如果你不改变方向，并且继续寻找，你可能最终会到达你所前往的地方。',
            linkLabel: '前往首页',
            linkText: '带我回首页'
        },
        langMenuLabel: '多语言',
        returnToTopLabel: '回到顶部',
        sidebarMenuLabel: '菜单',
        darkModeSwitchLabel: '主题',
        lightModeSwitchTitle: '切换到浅色模式',
        darkModeSwitchTitle: '切换到深色模式',
        skipToContentLabel: '跳转到内容'
    }
})

function searchOptions(): Partial<DefaultTheme.AlgoliaSearchOptions> {
    return {
        placeholder: '搜索文档',
        translations: {
            button: {
                buttonText: '搜索文档',
                buttonAriaLabel: '搜索文档'
            },
            modal: {
                searchBox: {
                    clearButtonTitle: '清除查询条件',
                    clearButtonAriaLabel: '清除查询条件',
                    closeButtonText: '关闭',
                    closeButtonAriaLabel: '关闭',
                    placeholderText: '搜索文档',
                    placeholderTextAskAi: '向 AI 提问：',
                    placeholderTextAskAiStreaming: '回答中...',
                    searchInputLabel: '搜索',
                    backToKeywordSearchButtonText: '返回关键字搜索',
                    backToKeywordSearchButtonAriaLabel: '返回关键字搜索'
                },
                startScreen: {
                    recentSearchesTitle: '搜索历史',
                    noRecentSearchesText: '没有搜索历史',
                    saveRecentSearchButtonTitle: '保存至搜索历史',
                    removeRecentSearchButtonTitle: '从搜索历史中移除',
                    favoriteSearchesTitle: '收藏',
                    removeFavoriteSearchButtonTitle: '从收藏中移除',
                    recentConversationsTitle: '最近的对话',
                    removeRecentConversationButtonTitle: '从历史记录中删除对话'
                },
                errorScreen: {
                    titleText: '无法获取结果',
                    helpText: '你可能需要检查你的网络连接'
                },
                noResultsScreen: {
                    noResultsText: '无法找到相关结果',
                    suggestedQueryText: '你可以尝试查询',
                    reportMissingResultsText: '你认为该查询应该有结果？',
                    reportMissingResultsLinkText: '点击反馈'
                },
                resultsScreen: {
                    askAiPlaceholder: '向 AI 提问： '
                },
                askAiScreen: {
                    disclaimerText: '答案由 AI 生成，可能不准确，请自行验证。',
                    relatedSourcesText: '相关来源',
                    thinkingText: '思考中...',
                    copyButtonText: '复制',
                    copyButtonCopiedText: '已复制！',
                    copyButtonTitle: '复制',
                    likeButtonTitle: '赞',
                    dislikeButtonTitle: '踩',
                    thanksForFeedbackText: '感谢你的反馈！',
                    preToolCallText: '搜索中...',
                    duringToolCallText: '搜索 ',
                    afterToolCallText: '已搜索',
                    aggregatedToolCallText: '已搜索'
                },
                footer: {
                    selectText: '选择',
                    submitQuestionText: '提交问题',
                    selectKeyAriaLabel: 'Enter 键',
                    navigateText: '切换',
                    navigateUpKeyAriaLabel: '向上箭头',
                    navigateDownKeyAriaLabel: '向下箭头',
                    closeText: '关闭',
                    backToSearchText: '返回搜索',
                    closeKeyAriaLabel: 'Esc 键',
                    poweredByText: '搜索提供者'
                }
            }
        }
    }
}
