/* eslint-disable tailwindcss/enforces-negative-arbitrary-values */
/* eslint-disable tailwindcss/no-contradicting-classname */
import { cn } from "@/lib/utils"

const cardContent = {
  title: "Lorem ipsum dolor",
  description:
    "Lorem ipsum dolor, sit amet consectetur adipisicing elit. Nostrum, hic ipsum! Qui dicta debitis aliquid quo molestias explicabo iure!",
}
const CardBody = ({ className = "p-4" }) => (
  <div className={cn("text-left", className)}>
    <h3 className="mb-1 text-lg font-bold text-gray-900 dark:text-gray-100">
      {cardContent.title}
    </h3>
    <p className="text-gray-700 dark:text-gray-300">
      {cardContent.description}
    </p>
  </div>
)
//======================================
export const SimpleCard_V1 = () => {
  const Ellipses = () => {
    const sharedClasses =
      "rounded-full outline outline-8 dark:outline-gray-950 sm:my-6 md:my-8 size-1 my-4 outline-gray-50 bg-green-400"
    return (
      <div className="absolute z-0 grid h-full w-full items-center gap-8 lg:grid-cols-2">
        <section className="absolute z-0 grid h-full w-full grid-cols-2 place-content-between">
          <div className={`${sharedClasses} -mx-[2.5px]`}></div>
          <div className={`${sharedClasses} -mx-[2px] place-self-end`}></div>
          <div className={`${sharedClasses} -mx-[2.5px]`}></div>
          <div className={`${sharedClasses} -mx-[2px] place-self-end`}></div>
        </section>
      </div>
    )
  }
  const Container = ({ children }: { children: React.ReactNode }) => (
    <div className="relative mx-auto w-full rounded-lg border border-dashed border-zinc-300 px-4 dark:border-zinc-800 sm:px-6 md:px-8">
      <div className="absolute left-0 top-4 -z-0 h-px w-full bg-zinc-400 dark:bg-zinc-700 sm:top-6 md:top-8"></div>
      <div className="absolute bottom-4 left-0 z-0 h-px w-full bg-zinc-400 dark:bg-zinc-700 sm:bottom-6 md:bottom-8"></div>
      <div className="relative w-full border-x border-zinc-400 dark:border-zinc-700">
        <Ellipses />
        <div className="relative z-20 mx-auto py-8">{children}</div>
      </div>
    </div>
  )
  return (
    <Container>
      <div className="center w-full p-3">
        <CardBody />
      </div>
    </Container>
  )
}
//======================================
export const SimpleCard_V2 = () => {
  const Line = ({ className = "" }) => (
    <div
      className={cn(
        "absolute -z-0 h-px w-full from-zinc-200 from-[1%] via-zinc-400 to-zinc-600 dark:from-zinc-900 dark:via-zinc-700 dark:to-zinc-500",
        className
      )}
    />
  )
  const Container = ({ children }: { children: React.ReactNode }) => (
    <div className="relative mx-auto w-full px-4 sm:px-6 md:px-8">
      <Line className="left-0 top-2 bg-gradient-to-l sm:top-4 md:top-6" />
      <Line className="bottom-2 left-0 bg-gradient-to-r sm:bottom-4 md:bottom-6" />

      <Line className="inset-y-0 right-2 h-full w-px bg-gradient-to-t sm:right-4 md:right-6" />
      <Line className="inset-y-0 left-2 h-full w-px bg-gradient-to-t sm:left-4 md:left-6" />
      <div className="relative z-20 mx-auto py-8">{children}</div>
    </div>
  )
  return (
    <Container>
      <div className="center w-full p-4">
        <CardBody />
      </div>
    </Container>
  )
}

//======================================
export const SimpleCard_V3 = () => {
  const Icon = ({ className, ...rest }: any) => {
    return (
      <svg
        xmlns="http://www.w3.org/2000/svg"
        fill="none"
        viewBox="0 0 24 24"
        width={24}
        height={24}
        strokeWidth="1"
        stroke="currentColor"
        {...rest}
        className={cn("absolute size-6 text-black dark:text-white", className)}
      >
        <path strokeLinecap="round" strokeLinejoin="round" d="M12 6v12m6-6H6" />
      </svg>
    )
  }
  return (
    <div>
      <div className="relative border border-dashed border-zinc-400 dark:border-zinc-700">
        <Icon className="-left-3 -top-3" />
        <Icon className="-right-3 -top-3" />
        <Icon className="-bottom-3 -left-3" />
        <Icon className="-bottom-3 -right-3" />
        <CardBody className="p-6" />
      </div>
    </div>
  )
}
//======================================Neubrutalism
export const SimpleCard_V4 = () => {
  return (
    <div>
      <div className="relative rounded-sm border-[0.5px] border-zinc-400 shadow-[4px_4px_0px_0px_rgba(0,0,0)] dark:border-white/70 dark:shadow-[3px_3px_0px_0px_rgba(255,255,255,0.7)]">
        <CardBody className="p-6" />
      </div>
    </div>
  )
}

//======================================
export const SimpleCard_V5 = () => {
  return (
    <div>
      {/* inner card */}
      <div className="rounded-sm border-[0.5px] border-zinc-300 p-2 dark:border-zinc-800">
        <div
          className={cn(
            "rounded-sm border bg-gradient-to-br",
            // light mode
            " border-zinc-300 from-white to-zinc-200/60 shadow-[2px_0_8px_rgba(0,_0,_0,_0.15)]",
            // dark mode
            "dark:border-zinc-900/50 dark:from-zinc-950 dark:to-zinc-900/60 dark:shadow-inner"
          )}
        >
          <CardBody />
        </div>
      </div>
    </div>
  )
}

//======================================3D
export const SimpleCard_V6 = () => {
  return (
    <div>
      <div className="relative rounded-xl border border-zinc-400 bg-zinc-50 shadow-[0px_5px_0px_0px_rgba(0,0,0,0.7)] dark:border-zinc-700 dark:bg-zinc-900/50 dark:shadow-[0px_4px_0px_0px_rgba(255,255,255,0.5)]">
        <CardBody className="p-6" />
      </div>
    </div>
  )
}

//======================================
export const SimpleCard_V7 = () => {
  const Icon = ({
    className,
    ...rest
  }: React.HTMLAttributes<HTMLDivElement>) => {
    return (
      <div
        {...rest}
        className={cn(
          "absolute size-6 border-zinc-700 dark:border-zinc-200",
          className
        )}
      />
    )
  }
  return (
    <div className="relative rounded-md border-2 border-zinc-100 dark:border-zinc-700">
      <Icon className="-left-0.5 -top-0.5 rounded-tl-md border-l-2 border-t-2" />
      <Icon className="-right-0.5 -top-0.5 rounded-tr-md border-r-2 border-t-2" />
      <Icon className="-bottom-0.5 -left-0.5 rounded-bl-md border-b-2 border-l-2" />
      <Icon className="-bottom-0.5 -right-0.5 rounded-br-md border-b-2 border-r-2" />
      <CardBody className="p-6" />
    </div>
  )
}
