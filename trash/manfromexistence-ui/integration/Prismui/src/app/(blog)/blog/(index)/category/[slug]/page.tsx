import { Metadata } from "next";
import { notFound } from "next/navigation";
import { allBlogPosts } from "content-collections";

import { constructMetadata } from "@/lib/blog/constructMetadata";
import { BLOG_CATEGORIES } from "@/lib/blog/content";
import { getBlurDataURL } from "@/lib/blog/images";
import BlogCard from "@/components/blog/blog-card";

interface BlogPost {
  title: string;
  summary: string;
  publishedAt: string;
  image: string;
  author: string;
  slug: string;
  mdx?: string;
  related?: string[];
  tableOfContents?: any;
  images?: any;
  tweetIds?: any;
  githubRepos?: any;
  categories?: string[];
  _meta?: any;
}

interface BlogPostWithBlur extends BlogPost {
  blurDataURL: string;
}

export async function generateStaticParams() {
  return BLOG_CATEGORIES.map((category) => ({
    slug: category.slug,
  }));
}

export async function generateMetadata({
  params,
}: {
  params: { slug: string };
}): Promise<Metadata | undefined> {
  const category = BLOG_CATEGORIES.find(
    (category) => category.slug === params.slug
  );
  if (!category) {
    return;
  }

  const { title, description } = category;

  return constructMetadata({
    title: `${title} – Prism UI Blog`,
    description,
    image: `/api/og/blog?title=${encodeURIComponent(
      title
    )}&summary=${encodeURIComponent(description)}`,
  });
}

export default async function BlogCategory({
  params,
}: {
  params: {
    slug: string;
  };
}) {
  const data = BLOG_CATEGORIES.find(
    (category) => category.slug === params.slug
  );
  if (!data) {
    notFound();
  }

  const articles: BlogPostWithBlur[] = await Promise.all(
    allBlogPosts
      .filter((post) => post.categories?.includes(data.slug))
      .sort((a, b) => b.publishedAt.localeCompare(a.publishedAt))
      .map(async (post) => ({
        title: post.title,
        summary: post.summary,
        publishedAt: post.publishedAt,
        image: post.image,
        author: post.author,
        slug: post.slug,
        categories: post.categories,
        blurDataURL: await getBlurDataURL(post.image),
      }))
  );

  return articles.map((article, idx) => (
    <BlogCard key={article.slug} data={article} priority={idx <= 1} />
  ));
}
